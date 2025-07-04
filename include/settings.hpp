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

#ifndef CARD_COUNTER_SETTINGS_HPP
#define CARD_COUNTER_SETTINGS_HPP

#include <QColor>
#include <QObject>

class Settings final : public QObject {
    Q_OBJECT

public:
    static Settings& instance();

    [[nodiscard]] bool indexing() const;
    [[nodiscard]] bool strategy_hint() const;
    [[nodiscard]] bool training() const;
    [[nodiscard]] bool show_time() const;
    [[nodiscard]] bool show_score() const;
    [[nodiscard]] bool show_speed() const;
    [[nodiscard]] bool infinity_mode() const;
    [[nodiscard]] QString card_theme() const;
    // [[nodiscard]]  QColor card_background() const;
    [[nodiscard]] QColor card_border() const;

public slots:
    void set_indexing(bool value);
    void set_strategy_hint(bool value);
    void set_training(bool value);
    void set_show_time(bool value);
    void set_show_score(bool value);
    void set_show_speed(bool value);
    void set_infinity_mode(bool value);
    void set_card_theme(const QString& value);
    // void set_card_background(const QColor& value);
    void set_card_border(const QColor& value);

signals:
    void indexing_changed(bool value);
    void strategy_hint_changed(bool value);
    void training_changed(bool value);
    void show_time_changed(bool value);
    void show_score_changed(bool value);
    void show_speed_changed(bool value);
    void infinity_mode_changed(bool value);
    void card_theme_changed(const QString& value);
    // void card_background_changed(const QColor& value);
    void card_border_changed(const QColor& value);

private:
    explicit Settings(QObject* parent = nullptr);

    bool indexing_ = false;
    bool strategy_hint_ = false;
    bool is_training_ = false;
    bool show_time_ = true;
    bool show_score_ = true;
    bool show_speed_ = true;
    bool infinity_mode_ = false;
    QString card_theme_ = "tigullio-international";
    // QColor card_background_ = Qt::white;
    QColor card_border_ = Qt::green;
};

#endif // CARD_COUNTER_SETTINGS_HPP