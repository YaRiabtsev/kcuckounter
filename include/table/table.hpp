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

#ifndef CARD_COUNTER_TABLE_HPP
#define CARD_COUNTER_TABLE_HPP

// Qt
#include <KGameDifficultyLevel>
#include <QSet>
#include <QWidget>

class QGridLayout;

class QSvgRenderer;

class TableSlot;

class StrategyInfo;

/**
 * @brief Widget hosting all table slots and game logic.
 */
class Table final : public QWidget {
    Q_OBJECT
public:
    explicit Table(QWidget* parent = nullptr);

    void create_new_game(KGameDifficultyLevel::StandardLevel level);

    void pause(bool paused);

    void force_game_over();

signals:

    void game_paused(bool paused);

    void table_slot_resized(QSize new_fixed_size);

    void can_remove(bool can_remove);

    void score_update(bool inc);

    void game_over();

private Q_SLOTS:

    void on_table_slot_activated();

    void on_table_slot_finished();

    void on_table_slot_removed();

    void on_table_slot_reshuffled();

    void on_user_quizzed();

    void on_user_answered(bool correct);

    void on_swap_target_selected();

    void pick_up_cards();

    void on_strategy_info_assist() const;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void add_new_table_slot(bool is_active = false);

    void set_renderer(const QString& card_theme);

    /**
     * @brief Determine how many columns can fit on screen.
     *
     * Calculates the optimal number of table slot columns and the
     * scaling factor for the given window size. The result is used to
     * reorganise the grid layout.
     *
     * @param table_size   available size of the table widget
     * @param aspect_ratio size of a single card (width/height)
     * @param item_count   number of table slots currently in play
     */
    void calculate_new_column_count(
        const QSizeF& table_size, const QSizeF& aspect_ratio, int item_count
    );

    /**
     * @brief Apply new layout parameters to all slots.
     */
    void reorganize_table(
        qint32 new_column_count, double new_scale, bool new_rotated = false
    );

    QGridLayout* layout;
    QSvgRenderer* renderer {};
    QRectF bounds;
    QTimer* countdown;
    StrategyInfo* strategy_info;

    bool launching {};
    qint32 column_count = -1;
    qint32 table_slot_count_limit {};
    qreal scale = -1;
    bool rotated = false;

    QVector<int> swap_target;
    QVector<TableSlot*> items;
    QSet<qint32> jokers;
    QSet<qint32> available;
};

#endif // CARD_COUNTER_TABLE_HPP
