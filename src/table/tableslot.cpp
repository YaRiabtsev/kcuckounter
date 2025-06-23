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
#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QSvgRenderer>
// KF
#include <KLocalizedString>
// own
#include "strategy/strategy.hpp"
#include "strategy/strategyinfo.hpp"
#include "table/tableslot.hpp"
#include "widgets/cards.hpp"
// own widgets
#include "widgets/base/frame.hpp"
#include "widgets/base/label.hpp"

TableSlot::TableSlot(
    StrategyInfo* strategies, QSvgRenderer* renderer, const bool is_active,
    QWidget* parent
)
    : Cards(renderer, parent)
    , strategies(strategies) {

    // QLabels:
    message_label = new CCLabel(i18n("TableSlot Weight: 0"));
    index_label = new CCLabel("0/0");
    weight_label = new CCLabel("weight: 0");
    strategy_hint_label = new CCLabel("");
    on_strategy_changed(0);

    // QComboBoxes:
    strategy_box = new QComboBox();
    on_new_strategy();
    connect(
        strategies, &StrategyInfo::new_strategy, this,
        &TableSlot::on_new_strategy
    );
    connect(
        strategy_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
        &TableSlot::on_strategy_changed
    );
    auto* indexing = new QCheckBox();
    connect(
        indexing, &QCheckBox::checkStateChanged, index_label,
        &CCLabel::setVisible
    );
    auto* strategy_hint = new QCheckBox();
    connect(
        strategy_hint, &QCheckBox::checkStateChanged, strategy_hint_label,
        &CCLabel::setVisible
    );
    auto* training = new QCheckBox();
    connect(
        training, &QCheckBox::checkStateChanged, weight_label,
        &CCLabel::setVisible
    );

    // QFrames:
    answer_frame = new CCFrame();
    settings_frame = new CCFrame();
    settings_frame->show();
    control_frame = new CCFrame();

    // QSpinBoxes:
    weight_box = new QSpinBox();
    weight_box->setRange(-100, 100);

    deck_count = new QSpinBox();
    connect(
        deck_count, QOverload<int>::of(&QSpinBox::valueChanged), this,
        &TableSlot::activate
    );
    deck_count->setRange(is_active, 10);

    // QPushButtons:
    auto* submit_button
        = new QPushButton(QIcon::fromTheme("answer"), i18n("&Submit"));
    submit_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(
        submit_button, &QPushButton::clicked, this, &TableSlot::user_checking
    );

    auto* skip_button = new QPushButton(
        QIcon::fromTheme("media-skip-forward"), i18n("&Skip")
    );
    skip_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    // todo:    connect(skipButton, &QPushButton::clicked, this,
    // &TableSlot::skipping);

    auto* strategy_info_button
        = new QPushButton(QIcon::fromTheme("kt-info-widget"), i18n("&Info"));
    strategy_info_button->setSizePolicy(
        QSizePolicy::Maximum, QSizePolicy::Maximum
    );
    connect(
        strategy_info_button, &QPushButton::clicked, this,
        &TableSlot::strategy_info_assist
    );

    close_button = new QPushButton(QIcon::fromTheme("delete"), i18n("&Remove"));
    close_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(
        close_button, &QPushButton::clicked, this,
        &TableSlot::table_slot_removed
    );
    close_button->hide();

    refresh_button
        = new QPushButton(QIcon::fromTheme("view-refresh"), i18n("&Reshuffle"));
    refresh_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(
        refresh_button, &QPushButton::clicked, this, &TableSlot::reshuffle_deck
    );
    refresh_button->hide();

    swap_button = new QPushButton(
        QIcon::fromTheme("exchange-positions"), i18n("&Swap")
    );
    swap_button->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    connect(
        swap_button, &QPushButton::clicked, this,
        &TableSlot::swap_target_selected
    );

    // QFormLayouts:
    auto* settings = new QFormLayout(settings_frame);
    settings->setFormAlignment(Qt::AlignCenter);
    auto* answer = new QFormLayout(answer_frame);
    answer->setFormAlignment(Qt::AlignCenter);

    // Other Layouts:
    auto* box_layout = new QVBoxLayout(this);
    auto* info_layout = new QHBoxLayout();
    auto* control_layout = new QHBoxLayout(control_frame);
    auto* strategy_layout = new QHBoxLayout();

    strategy_layout->addWidget(strategy_box);
    strategy_layout->addWidget(strategy_info_button);

    info_layout->addWidget(weight_label);
    info_layout->addStretch();
    info_layout->addWidget(index_label);

    answer->addRow(tr("&Weight:"), weight_box);
    answer->addRow(submit_button, skip_button);

    settings->addRow(tr("&Number of Card Decks:"), deck_count);
    settings->addRow(tr("Type of Strategy:"), strategy_layout);
    settings->addRow(indexing, new QLabel("Use card indexing"));
    settings->addRow(strategy_hint, new QLabel("Add name of strategy"));
    settings->addRow(training, new QLabel("Is training"));

    control_layout->addWidget(close_button);
    control_layout->addWidget(refresh_button);
    control_layout->addWidget(swap_button);

    box_layout->addWidget(strategy_hint_label);
    box_layout->addStretch();
    box_layout->addWidget(message_label);
    box_layout->addWidget(answer_frame);
    box_layout->addWidget(settings_frame);
    box_layout->addWidget(control_frame);
    box_layout->addStretch();
    box_layout->addLayout(info_layout);
}

void TableSlot::on_game_paused(const bool paused) {
    if (!settings_frame->isHidden()) {
        cards = shuffle_cards(deck_count->value());
        refresh_button->show();
        //        swapButton->hide();
        set_id(-1);
        settings_frame->hide();
    }
    if (paused) {
        answer_frame->hide();
        set_name("blue_back");
        control_frame->show();
    } else {
        control_frame->hide();
        if (is_joker()) {
            set_name(get_card_name_by_current_id());
            user_quizzing();
        }
    }
    update();
}

bool TableSlot::is_fake() const { return fake; }

void TableSlot::pick_up_card() {
    if (cards.empty()) {
        set_name("back");
        emit table_slot_finished();
        settings_frame->show();
        control_frame->show();
        update();
        return;
    }
    //    if (isJoker()){
    //        messageLabel->hide();
    //    }
    set_id(cards.front());
    set_name(get_card_name_by_current_id());
    cards.pop_front();
    if (!message_label->isHidden()) {
        message_label->hide();
    }
    update();
    index_label->setText(i18n(
        "%1/%2", deck_count->value() * 54 - cards.size(),
        deck_count->value() * 54
    ));
    if (is_joker()) {
        user_quizzing();
    } else {
        current_weight
            = strategy->update_weight(current_weight, get_current_rank());
        weight_label->setText(i18n("weight: %1", current_weight));
    }
    // add highlighting
}

void TableSlot::user_quizzing() {
    answer_frame->show();
    emit user_quizzed();
}

void TableSlot::user_checking() {
    message_label->setText(i18n("TableSlot Weight: %1", current_weight));
    answer_frame->hide();
    const bool is_correct = weight_box->value() == current_weight;
    message_label->setPalette(QPalette(is_correct ? Qt::green : Qt::red));
    message_label->show();
    emit user_answered(is_correct);
}

void TableSlot::reshuffle_deck() {
    cards = shuffle_cards(deck_count->value());
    settings_frame->hide();
    // hide controlFrame if not paused
}

void TableSlot::on_can_remove(const bool can_remove) const {
    close_button->setVisible(can_remove);
}

void TableSlot::activate(const int value) {
    if (value > 0 && fake) {
        fake = false;
        control_frame->show();
        set_name("green_back");
        current_weight = 0;
        deck_count->setMinimum(1);
        emit table_slot_activated();
    }
}

void TableSlot::on_new_strategy() const {
    QStringList items;
    for (const auto& item : strategies->get_strategies()) {
        items.push_back(item->get_name());
    }
    items.pop_back(); // last is fake
    strategy_box->clear();
    strategy_box->addItems(items);
}

void TableSlot::on_strategy_changed(const int index) {
    if (index >= 0) {
        strategy = strategies->get_strategy_by_id(index);
        strategy_hint_label->setText(strategy->get_name());
    }
}
