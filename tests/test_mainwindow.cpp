/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 Yaroslav Riabtsev <yaroslav.riabtsev@rwth-aachen.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "mainwindow.hpp"
#include "table/table.hpp"

#include <QStatusBar>
#include <QtTest/QtTest>

class TestMainWindow final : public QObject {
    Q_OBJECT
private slots:
    static void initial_state();
    static void score_update();
    static void advance_time();
    static void pause_game_signal();
    void end_game_signal();
};

void TestMainWindow::initial_state() {
    const MainWindow window;
    const auto labels = window.statusBar()->findChildren<QLabel*>();
    QCOMPARE(labels.at(0)->text(), QStringLiteral("Score: 0/0"));
    QCOMPARE(labels.at(1)->text(), QStringLiteral("Time: 00:00"));
}

void TestMainWindow::score_update() {
    MainWindow window;
    const auto labels = window.statusBar()->findChildren<QLabel*>();
    QMetaObject::invokeMethod(&window, "on_score_update", Q_ARG(bool, true));
    QMetaObject::invokeMethod(&window, "on_score_update", Q_ARG(bool, false));
    QCOMPARE(labels[0]->text(), QStringLiteral("Score: 1/2"));
}

void TestMainWindow::advance_time() {
    MainWindow window;
    const auto labels = window.statusBar()->findChildren<QLabel*>();
    QMetaObject::invokeMethod(
        &window, "advance_time", Qt::DirectConnection,
        Q_ARG(QString, QStringLiteral("01:20"))
    );
    QCOMPARE(labels[1]->text(), QStringLiteral("Time: 01:20"));
}

void TestMainWindow::pause_game_signal() {
    MainWindow window;
    QSignalSpy spy(window.findChild<Table*>(), &Table::game_paused);
    QMetaObject::invokeMethod(&window, "pause_game", Q_ARG(bool, true));
    QMetaObject::invokeMethod(&window, "pause_game", Q_ARG(bool, false));
    QCOMPARE(spy.count(), 2);
    QCOMPARE(spy.takeFirst().at(0).toBool(), true);
    QCOMPARE(spy.takeFirst().at(0).toBool(), false);
}

void TestMainWindow::end_game_signal() {
    MainWindow window;
    QSignalSpy spy(window.findChild<Table*>(), &Table::game_over);
    QMetaObject::invokeMethod(&window, "force_end_game");
    QCOMPARE(spy.count(), 1);
}

#include "test_mainwindow.moc"