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

// Qt
#include <QCloseEvent>
#include <QMessageBox>
#include <QStatusBar>
// KDEGames
#include <KGameClock>
#include <KGameHighScoreDialog>
#include <KGameStandardAction>
// KF
#include <KActionCollection>
#include <KLocalizedString>
// own
#include "mainwindow.hpp"
#include "table/table.hpp"

MainWindow::MainWindow(QWidget* parent)
    : KXmlGuiWindow(parent) {
    game_clock = new KGameClock(this, KGameClock::FlexibleHourMinSec);
    connect(
        game_clock, &KGameClock::timeChanged, this, &MainWindow::advance_time
    );

    score_label->setText(i18n("Score: 0/0"));
    time_label->setText(i18n("Time: 00:00"));

    statusBar()->insertPermanentWidget(0, score_label);
    statusBar()->insertPermanentWidget(1, time_label);

    table = new Table;
    connect(table, &Table::score_update, this, &MainWindow::on_score_update);
    connect(table, &Table::game_over, this, &MainWindow::on_game_over);

    setCentralWidget(table);
    setup_actions();
    new_game();
}

void MainWindow::setup_actions() {
    KGameStandardAction::gameNew(
        this, &MainWindow::new_game, actionCollection()
    );
    KGameStandardAction::highscores(
        this, &MainWindow::show_high_scores, actionCollection()
    );

    KGameStandardAction::quit(this, &MainWindow::close, actionCollection());
    KStandardAction::preferences(
        this, &MainWindow::configure_settings, actionCollection()
    );
    action_pause = KGameStandardAction::pause(
        this, &MainWindow::pause_game, actionCollection()
    );
    action_end_game = actionCollection()->addAction(QStringLiteral("game_end"));
    action_end_game->setText(i18n("End Game"));
    connect(
        action_end_game, &QAction::triggered, this, &MainWindow::force_end_game
    );

    KGameDifficulty::global()->addStandardLevelRange(
        KGameDifficultyLevel::Easy, KGameDifficultyLevel::Hard,
        KGameDifficultyLevel::Easy
    );
    KGameDifficulty::global()->addLevel(new KGameDifficultyLevel(
        1000, QByteArray("Nightmare"), i18n("Nightmare")
    ));
    KGameDifficultyGUI::init(this);
    connect(
        KGameDifficulty::global(), &KGameDifficulty::currentLevelChanged, this,
        &MainWindow::new_game
    );

    setupGUI(Default, "kcuckounterui.rc");
}

void MainWindow::new_game() {
    if (score.second > 0) {
        const auto res = QMessageBox::question(
            this, i18n("End Game"),
            i18n("End current game and save the result?"),
            QMessageBox::Yes | QMessageBox::No
        );
        if (res == QMessageBox::No) {
            return;
        }
        force_end_game();
    }
    game_clock->restart();
    game_clock->pause();
    table->create_new_game(KGameDifficulty::globalLevel());
    if (!action_pause->isChecked()) {
        action_pause->setChecked(true);
    }
    score = { 0, 0 };
    score_label->setText(i18n("Score: 0/0"));
    KGameDifficulty::global()->setGameRunning(false);
    time_label->setText(i18n("Time: 00:00"));
}

void MainWindow::force_end_game() const { table->force_game_over(); }

void MainWindow::closeEvent(QCloseEvent* event) {
    if (score.second > 0) {
        const auto res = QMessageBox::question(
            this, i18n("Quit"), i18n("End current game and save the result?"),
            QMessageBox::Yes | QMessageBox::No
        );
        if (res == QMessageBox::No) {
            event->ignore();
            return;
        }
        force_end_game();
    }
    event->accept();
}

void MainWindow::on_game_over() {
    game_clock->pause();
    action_pause->setEnabled(false);
    KGameDifficulty::global()->setGameRunning(false);
    QPointer scoreDialog = new KGameHighScoreDialog(
        KGameHighScoreDialog::Name | KGameHighScoreDialog::Time, this
    );
    scoreDialog->initFromDifficulty(KGameDifficulty::global());
    KGameHighScoreDialog::FieldInfo scoreInfo;
    scoreInfo[KGameHighScoreDialog::Score]
        = i18n("%1/%2", score.first, score.second);
    scoreInfo[KGameHighScoreDialog::Time] = game_clock->timeString();

    if (scoreDialog->addScore(scoreInfo, KGameHighScoreDialog::LessIsMore) != 0)
        scoreDialog->exec();

    delete scoreDialog;
}

void MainWindow::advance_time(const QString& elapsed_time) const {
    time_label->setText(i18n("Time: %1", elapsed_time));
}

void MainWindow::show_high_scores() {
    QPointer score_dialog = new KGameHighScoreDialog(
        KGameHighScoreDialog::Name | KGameHighScoreDialog::Time, this
    );
    score_dialog->initFromDifficulty(KGameDifficulty::global());
    score_dialog->exec();
    delete score_dialog;
}

void MainWindow::configure_settings() { }

void MainWindow::pause_game(const bool paused) const {
    table->pause(paused);
    if (paused) {
        game_clock->pause();
    } else {
        game_clock->resume();
    }
}

void MainWindow::load_settings() { }

void MainWindow::on_score_update(const bool inc) {
    score.second++;
    score.first += inc;
    score_label->setText(i18n("Score: %1/%2", score.first, score.second));
}
