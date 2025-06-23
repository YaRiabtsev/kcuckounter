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

#include "widgets/cards.hpp"
#include <QtTest/QtTest>

class TestCards final : public QObject {
    Q_OBJECT
private slots:
    static void deck_generation_size();
    static void card_name();
    static void rank_suit_extraction();
    static void joker_detection();
};

void TestCards::deck_generation_size() {
    const auto deck = Cards::generate_deck(2);
    QCOMPARE(deck.size(), 2 * (52 + 2));
}

void TestCards::card_name() {
    constexpr qint32 card = ((Cards::Spades & 0xff) << 8) | Cards::Ace;
    QCOMPARE(Cards::card_name(card, 1), QStringLiteral("ace_spade"));
}

void TestCards::rank_suit_extraction() {
    constexpr qint32 card = ((Cards::Hearts & 0xff) << 8) | Cards::Ten;
    QCOMPARE(Cards::get_suit(card), Cards::Hearts);
    QCOMPARE(Cards::get_rank(card), Cards::Ten);
    QVERIFY(!Cards::is_joker(card));
}

void TestCards::joker_detection() {
    constexpr qint32 joker = ((Cards::Red & 0xff) << 8);
    QVERIFY(Cards::is_joker(joker));
}

#include "test_cards.moc"