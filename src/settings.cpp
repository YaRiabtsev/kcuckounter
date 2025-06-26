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
#include "settings.hpp"

Settings::Settings(QObject* parent)
    : QObject(parent) { }

Settings& Settings::instance() {
    static Settings inst;
    return inst;
}

bool Settings::indexing() const { return indexing_; }

bool Settings::strategy_hint() const { return strategy_hint_; }

bool Settings::training() const { return is_training_; }

bool Settings::show_time() const { return show_time_; }

bool Settings::show_score() const { return show_score_; }

bool Settings::show_speed() const { return show_speed_; }

QString Settings::card_theme() const { return card_theme_; }

// QColor Settings::card_background() const { return card_background_; }

QColor Settings::card_border() const { return card_border_; }

void Settings::set_indexing(const bool value) {
    if (indexing_ != value) {
        indexing_ = value;
        emit indexing_changed(value);
    }
}

void Settings::set_strategy_hint(const bool value) {
    if (strategy_hint_ != value) {
        strategy_hint_ = value;
        emit strategy_hint_changed(value);
    }
}

void Settings::set_training(const bool value) {
    if (is_training_ != value) {
        is_training_ = value;
        emit training_changed(value);
    }
}

void Settings::set_show_time(const bool value) {
    if (show_time_ != value) {
        show_time_ = value;
        emit show_time_changed(value);
    }
}

void Settings::set_show_score(const bool value) {
    if (show_score_ != value) {
        show_score_ = value;
        emit show_score_changed(value);
    }
}

void Settings::set_show_speed(const bool value) {
    if (show_speed_ != value) {
        show_speed_ = value;
        emit show_speed_changed(value);
    }
}

void Settings::set_card_theme(const QString& value) {
    if (card_theme_ != value) {
        card_theme_ = value;
        emit card_theme_changed(value);
    }
}

// void Settings::set_card_background(const QColor& value) {
//     if (card_background_ != value) {
//         card_background_ = value;
//         emit card_background_changed(value);
//     }
// }

void Settings::set_card_border(const QColor& value) {
    if (card_border_ != value) {
        card_border_ = value;
        emit card_border_changed(value);
    }
}
