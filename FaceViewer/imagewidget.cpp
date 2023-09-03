// ============================================================================================== //
//                                                                                                //
//  This file is part of the ISF Face Detector project.                                           //
//                                                                                                //
//  Author:                                                                                       //
//  Marcel Hasler <mahasler@gmail.com>                                                            //
//                                                                                                //
//  Copyright (c) 2021 - 2023                                                                     //
//  Bonn-Rhein-Sieg University of Applied Sciences                                                //
//                                                                                                //
//  This program is free software: you can redistribute it and/or modify it under the terms of    //
//  the GNU General Public License as published by the Free Software Foundation, either           //
//  version 3 of the License, or (at your option) any later version.                              //
//                                                                                                //
//  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     //
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     //
//  See the GNU General Public License for more details.                                          //
//                                                                                                //
//  You should have received a copy of the GNU General Public License along with this program.    //
//  If not, see <https://www.gnu.org/licenses/>.                                                  //
//                                                                                                //
// ============================================================================================== //

#include "imagewidget.h"

#include <QPainter>

#include <cmath> // for round()

// ---------------------------------------------------------------------------------------------- //

ImageWidget::ImageWidget(QWidget* parent)
    : QWidget(parent)
{
}

// ---------------------------------------------------------------------------------------------- //

void ImageWidget::setImage(const QImage& image)
{
    m_image = image;
    update();
}

// ---------------------------------------------------------------------------------------------- //

void ImageWidget::setRects(const std::vector<QRect>& rects)
{
    if (m_rects.size() != rects.size())
    {
        m_rects.clear();
        m_rects.resize(rects.size());
    }

    for (size_t i = 0; i < m_rects.size(); ++i)
        m_rects[i].addRect(rects[i]);

    update();
}

// ---------------------------------------------------------------------------------------------- //

void ImageWidget::setMirrored(bool mirror)
{
    m_mirrored = mirror;
    update();
}

// ---------------------------------------------------------------------------------------------- //

void ImageWidget::clear()
{
    setImage(QImage());
    m_rects.clear();
}

// ---------------------------------------------------------------------------------------------- //

void ImageWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    const QRect& widgetRect = rect();

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    painter.setPen(Qt::black);
    painter.setBrush(Qt::black);
    painter.drawRect(widgetRect);

    if (m_image.isNull())
        return;

    if (m_mirrored)
    {
        painter.translate(widgetRect.width(), 0.0);
        painter.scale(-1.0, 1.0);
    }

    const auto pixmap = QPixmap::fromImage(m_image);

    QSize pixmapSize = pixmap.size();
    pixmapSize.scale(widgetRect.size(), Qt::KeepAspectRatio);

    QRect pixmapRect(QPoint(), pixmapSize);
    pixmapRect.moveCenter(widgetRect.center());

    painter.drawPixmap(pixmapRect, pixmap);

    const QPen pen(Qt::green, 2.0);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    const double scale = 1.0 * pixmapRect.width() / pixmap.width();

    for (const auto& average : m_rects)
    {
        const QRect rect = average.getAverage();
        const auto x = static_cast<int>(std::round(pixmapRect.x() + scale * rect.x()));
        const auto y = static_cast<int>(std::round(pixmapRect.y() + scale * rect.y()));
        const auto w = static_cast<int>(std::round(scale * rect.width()));
        const auto h = static_cast<int>(std::round(scale * rect.height()));

        painter.drawRect(x, y, w, h);
    }
}

// ---------------------------------------------------------------------------------------------- //
