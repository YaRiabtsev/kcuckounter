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

#ifndef CARD_COUNTER_MAINWINDOW_HPP
#define CARD_COUNTER_MAINWINDOW_HPP

// KF
#include <KXmlGuiWindow>
// Qt
#include <QLabel>
#include <QPointer>
#include <QSlider>

class KGameClock;

class KToggleAction;

class Table;

class MainWindow final : public KXmlGuiWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private Q_SLOTS:

    void advance_time(const QString& elapsed_time) const;

    void load_settings() const;

    void new_game();

    void force_end_game() const;

    void on_game_over();

    void show_high_scores();

    void configure_settings();

    void pause_game(bool paused) const;

    void on_score_update(bool inc);

private:
    void setup_actions();

    void closeEvent(QCloseEvent* event) override;

    Table* table;

    KGameClock* game_clock = nullptr;
    KToggleAction* action_pause = nullptr;

    QPointer<QLabel> time_label = new QLabel;
    QPointer<QLabel> score_label = new QLabel;
    QPointer<QSlider> speed_slider = new QSlider(Qt::Horizontal);

    QPair<qint32, qint32> score;
    QAction* action_end_game = nullptr;
};

#endif // CARD_COUNTER_MAINWINDOW_HPP
