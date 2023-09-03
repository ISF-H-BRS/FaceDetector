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

#pragma once

#include <QRect>

#include <deque>
#include <vector>

class RectAverager
{
public:
    static constexpr size_t MinimumBufferSize = 3;
    static constexpr size_t DefautBufferSize = 5;

public:
    RectAverager(size_t bufferSize = DefautBufferSize);

    void setBufferSize(size_t size);

    void addRect(const QRect& rect);
    void reset();

    auto getAverage() const -> QRect;

private:
    std::deque<QRect> m_rects;

    std::vector<double> m_coeffs;
    size_t m_bufferSize = DefautBufferSize;
};
