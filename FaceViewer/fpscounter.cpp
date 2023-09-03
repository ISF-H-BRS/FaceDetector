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

#include "fpscounter.h"

#include <cmath> // for round()

// ---------------------------------------------------------------------------------------------- //

void FpsCounter::reset()
{
    m_times.clear();
}

// ---------------------------------------------------------------------------------------------- //

auto FpsCounter::update() -> int
{
    const auto now = Clock::now();

    m_times.push_back(now);

    while (m_times.size() > MaxDurationSize)
        m_times.pop_front();

    const auto diff = m_times.back() - m_times.front();

    if (diff.count() == 0)
        return 0;

    const auto frames = m_times.size() - 1;
    const auto fps = static_cast<double>(Clock::duration::period::den * frames) /
                     static_cast<double>(diff.count());

    return static_cast<int>(std::round(fps));
}

// ---------------------------------------------------------------------------------------------- //
