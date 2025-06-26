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

#include "test_cards.cpp"
// #include "test_mainwindow.cpp"
#include "test_strategy.cpp"
#include "test_table.cpp"

#include <KLocalizedString>

#include <QApplication>
#include <QStandardPaths>

int main(int argc, char* argv[]) {
    QStandardPaths::setTestModeEnabled(true);
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("kcuckounter"));
    KLocalizedString::setApplicationDomain("kcuckounter");
    int status = 0;

    TestStrategy strategy_test;
    status |= QTest::qExec(&strategy_test, argc, argv);

    TestCards cards_test;
    status |= QTest::qExec(&cards_test, argc, argv);

    TestTable table_test;
    status |= QTest::qExec(&table_test, argc, argv);

    // TestMainWindow test_mainwindow;
    // status |= QTest::qExec(&test_mainwindow, argc, argv);

    return status;
}
