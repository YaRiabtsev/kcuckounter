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
#include <QBoxLayout>
#include <QPushButton>
// own
#include "widgets/carousel.hpp"

Carousel::Carousel(const QSizeF aspect_ratio, QWidget* parent)
    : QWidget(parent)
    , ratio(aspect_ratio) {

    auto* box_layout = new QHBoxLayout(this);
    auto* carousel_box = new QWidget;
    carousel_box->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    carousel = new QHBoxLayout();
    carousel_box->setLayout(carousel);

    auto* back = new QPushButton();
    back->setIcon(QIcon::fromTheme("draw-arrow-back"));
    back->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(back, &QPushButton::clicked, [this] {
        idx = (idx + widgets.size() - 1) % widgets.size();
        update_layout();
    });

    auto* next = new QPushButton();
    next->setIcon(QIcon::fromTheme("draw-arrow-forward"));
    next->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
    connect(next, &QPushButton::clicked, [this] {
        idx = (idx + 1) % widgets.size();
        update_layout();
    });

    box_layout->addWidget(back);
    box_layout->addStretch();
    box_layout->addWidget(carousel_box);
    box_layout->addStretch();
    box_layout->addWidget(next);

    update_props(size());
}

void Carousel::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);

    update_props(size());
}

void Carousel::add_widget(QWidget* widget) {
    connect(
        this, &Carousel::item_resized, widget,
        [widget](const QSize newFixedSize) {
            widget->setFixedSize(newFixedSize);
            widget->update();
        }
    );
    widgets.push_back(widget);
    update_props(size());
}

void Carousel::update_props(const QSize size) {
    const auto item_size
        = QSizeF(size.height() * ratio.width() / ratio.height(), size.height());
    column_count = qMin(
        widgets.size(),
        static_cast<qint32>(0.95 * size.width() / item_size.width())
    );
    update_layout(0.9 * size.height() / ratio.height());
}

void Carousel::update_layout(const double new_scale) {
    while (carousel->count()) {
        const QLayoutItem* item = carousel->takeAt(0);
        item->widget()->hide();
        delete item;
    }

    if (new_scale > 0) {
        const QSizeF new_fixed_size(
            ratio.width() * new_scale, ratio.height() * new_scale
        );
        emit item_resized(new_fixed_size.toSize());
    }

    for (qint32 i = 0; i < column_count; i++) {
        QWidget* item = widgets[(idx + i) % widgets.size()];
        carousel->addWidget(item);
        item->show();
    }
}
