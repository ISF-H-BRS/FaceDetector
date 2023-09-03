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

#include "rectaverager.h"

#include <cassert>
#include <cmath>
#include <numeric>

// ---------------------------------------------------------------------------------------------- //

static auto operator+(const QRect& lhs, const QRect& rhs) -> QRect
{
    return {
        lhs.x() + rhs.x(),
        lhs.y() + rhs.y(),
        lhs.width() + rhs.width(),
        lhs.height() + rhs.height()
    };
}

// ---------------------------------------------------------------------------------------------- //

static auto operator*(double lhs, const QRect& rhs) -> QRect
{
    return {
        static_cast<int>(std::round(lhs * rhs.x())),
        static_cast<int>(std::round(lhs * rhs.y())),
        static_cast<int>(std::round(lhs * rhs.width())),
        static_cast<int>(std::round(lhs * rhs.height()))
    };
}

// ---------------------------------------------------------------------------------------------- //

RectAverager::RectAverager(size_t bufferSize)
{
    setBufferSize(bufferSize);
}

// ---------------------------------------------------------------------------------------------- //

void RectAverager::setBufferSize(size_t size)
{
    assert(size >= MinimumBufferSize && size % 2 == 1);

    m_bufferSize = size;
    m_coeffs.resize(m_bufferSize);

    // Savitzky-Golay smoothing
    const auto m = static_cast<double>(m_bufferSize);
    const auto shift = static_cast<double>(m_bufferSize - 1) / 2;

    for (size_t index = 0; index < size; ++index)
    {
        const auto i = static_cast<double>(index) - shift;
        m_coeffs[index] = ((3.0*m*m - 7.0 - 20.0*i*i) / 4.0) / (m*(m*m - 4.0) / 3.0);
    }
}

// ---------------------------------------------------------------------------------------------- //

void RectAverager::addRect(const QRect& rect)
{
    while (m_rects.size() <= m_bufferSize)
        m_rects.push_back(rect);

    while (m_rects.size() > m_bufferSize)
        m_rects.pop_front();
}

// ---------------------------------------------------------------------------------------------- //

void RectAverager::reset()
{
    m_rects.clear();
}

// ---------------------------------------------------------------------------------------------- //

auto RectAverager::getAverage() const -> QRect
{
    assert(m_rects.size() == m_bufferSize);
    assert(m_rects.size() == m_coeffs.size());

    QRect result;

    for (size_t i = 0; i < m_bufferSize; ++i)
        result = result + m_coeffs[i] * m_rects[i];

    return result;
}

// ---------------------------------------------------------------------------------------------- //
