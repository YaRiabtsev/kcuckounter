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

#include "strategy/strategy.hpp"
#include <QtTest/QtTest>

class TestStrategy : public QObject {
    Q_OBJECT
private slots:
    void basicProperties();
    void updateWeight();
};

void TestStrategy::basicProperties() {
    QVector<qint32> weights { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
    Strategy s("name", "desc", weights, true);
    QVERIFY(s.is_custom());
    QCOMPARE(s.get_name(), QStringLiteral("name"));
    QCOMPARE(s.get_description(), QStringLiteral("desc"));
    QCOMPARE(s.get_weights(0), 1);
    QCOMPARE(s.get_weights(12), 13);
}

void TestStrategy::updateWeight() {
    QVector<qint32> weights { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    Strategy s("n", "d", weights, true);
    QCOMPARE(s.update_weight(0, 1), 1);
    QCOMPARE(s.update_weight(1, 1), 2);
}

#include "test_strategy.moc"