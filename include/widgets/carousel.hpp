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

#ifndef CARD_COUNTER_CAROUSEL_HPP
#define CARD_COUNTER_CAROUSEL_HPP

// Qt
#include <QWidget>

class QHBoxLayout;

/**
 * @brief A simple horizontal carousel for displaying widgets.
 */
class Carousel final : public QWidget {
    Q_OBJECT
public:
    explicit Carousel(QSizeF aspect_ratio, QWidget* parent = nullptr);

    /**
     * @brief Append a widget to the carousel.
     */
    void add_widget(QWidget* widget);

protected:
    void resizeEvent(QResizeEvent* event) override;

signals:

    /** Emitted whenever the size for child widgets changes. */
    void item_resized(QSize new_fixed_size);

private:
    void update_props(QSize size);

    /**
     * @brief Rearrange visible items according to current index.
     */
    void update_layout(double new_scale = -1);

    QHBoxLayout* carousel;
    QVector<QWidget*> widgets;
    QSizeF ratio;
    qint32 column_count = 0;
    qint32 idx = 0;
};

#endif // CARD_COUNTER_CAROUSEL_HPP
