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

#ifndef CARD_COUNTER_CARDS_HPP
#define CARD_COUNTER_CARDS_HPP

// Qt
#include <QWidget>

class QSvgRenderer;

/**
 * @brief A widget representing a playing card.
 *
 * The class stores the current card id and renders the SVG
 * corresponding to that card. It also provides a number of
 * helper functions for working with card decks.
 */
class Cards : public QWidget {
    Q_OBJECT
public:
    explicit Cards(QSvgRenderer* renderer, QWidget* parent = nullptr);

    void paintEvent(QPaintEvent* event) override;

    void set_id(qint32 id);

    void set_name(QString name);

    enum colour { Black = 0, Red };

    enum suit { Clubs = 0, Diamonds, Hearts, Spades };

    enum rank {
        Joker = 0,
        Ace,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight,
        Nine,
        Ten,
        Jack,
        Queen,
        King
    };

    /**
     * @brief Generate a shuffled deck.
     *
     * Jokers are distributed so that the distance between two jokers is at
     * least `deck_size / (deck_count * shuffle_coefficient)`.
     *
     * @param deck_count         number of standard 54 card decks
     * @param shuffle_coefficient larger values produce better joker spacing
     * @return list of card ids in random order
     */
    [[nodiscard]] static QList<qint32>
    shuffle_cards(qint32 deck_count, qint32 shuffle_coefficient = 2);

    /**
     * @brief Create an ordered deck of playing cards.
     *
     * The returned list contains 52 standard cards plus two jokers per deck.
     *
     * @param deck_count number of decks to generate
     */
    [[nodiscard]] static QList<qint32> generate_deck(qint32 deck_count);

    /**
     * @brief Convert a card id to its textual name.
     *
     * @param id       encoded card id
     * @param standard if set, use "joker" instead of "jocker" and "ace"
     * @return         human readable name (e.g. "ace_spade")
     */
    [[nodiscard]] static QString card_name(qint32 id, qint32 standard = 0);

    /**
     * @brief Name of the currently displayed card.
     *
     * Convenience wrapper around @ref card_name for the internally stored id.
     */
    [[nodiscard]] QString
    get_card_name_by_current_id(qint32 standard = 0) const;

    /**
     * @brief Check whether a card id represents a joker.
     */
    [[nodiscard]] static bool is_joker(qint32 id) noexcept;

    /**
     * @brief Check whether the current card is a joker.
     */
    [[nodiscard]] bool is_joker() const noexcept;

    /**
     * @brief Return textual representation of a card colour.
     */
    [[nodiscard]] static QString get_colour_name(qint32 colour);

    /**
     * @brief Return textual representation of a suit.
     */
    [[nodiscard]] static QString get_suit_name(qint32 suit);

    /**
     * @brief Return textual representation of a rank.
     *
     * @param rank     numeric rank value
     * @param standard choose between "ace" and numeric representation
     */
    [[nodiscard]] static QString
    get_rank_name(qint32 rank, bool standard = true);

    /**
     * @brief Extract the rank from a card id.
     */
    [[nodiscard]] static qint32 get_rank(qint32 id) noexcept;

    /**
     * @brief Rank of the currently stored card.
     */
    [[nodiscard]] qint32 get_current_rank() const noexcept;

    /**
     * @brief Extract the suit/colour from a card id.
     */
    [[nodiscard]] static qint32 get_suit(qint32 id) noexcept;

    /**
 * @brief Rotate the SVG by 90 degrees.
 *
 * When enabled the card image is painted in landscape
 * orientation without changing the widget geometry.
 */
    void set_rotated(bool rotated);


private:
    QString svg_name;
    qint32 current_card_id;

    QSvgRenderer* renderer;
    bool rotated_svg = false;
};

#endif // CARD_COUNTER_CARDS_HPP
