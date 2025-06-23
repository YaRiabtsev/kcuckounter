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

#ifndef CARD_COUNTER_STRATEGY_HPP
#define CARD_COUNTER_STRATEGY_HPP

// Qt
#include <QString>
#include <QVector>

/**
 * @brief Card counting strategy definition.
 */
class Strategy {
public:
    /**
     * @brief Construct a new strategy.
     * @param name        display name
     * @param description markdown description text
     * @param weights     weight for ranks Ace..King
     * @param custom      whether the strategy was created by the user
     */
    explicit Strategy(
        QString name, QString description, QVector<qint32> weights,
        bool custom = false
    );

    /** Whether this strategy was defined by the user. */
    [[nodiscard]] bool is_custom() const noexcept;

    /** Strategy name. */
    [[nodiscard]] QString get_name() const;

    /** Strategy description. */
    [[nodiscard]] QString get_description() const;

    /** Weight for the given rank index (0 based). */
    [[nodiscard]] qint32 get_weights(qint32 id) const;

    /**
     * @brief Update the current running count with a card rank.
     */
    qint32 update_weight(qint32 current_weight, qint32 rank);

private:
    bool custom;
    QVector<qint32> weights;
    QString name;
    QString description;
};

#endif // CARD_COUNTER_STRATEGY_HPP
