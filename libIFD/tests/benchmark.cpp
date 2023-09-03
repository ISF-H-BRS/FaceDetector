// ============================================================================================== //
//                                                                                                //
//  This file is part of the ISF Face Detector library.                                           //
//                                                                                                //
//  Author:                                                                                       //
//  Marcel Hasler <mahasler@gmail.com>                                                            //
//                                                                                                //
//  Copyright (c) 2021 - 2023                                                                     //
//  Bonn-Rhein-Sieg University of Applied Sciences                                                //
//                                                                                                //
//  This library is free software: you can redistribute it and/or modify it under the terms of    //
//  the GNU Lesser General Public License as published by the Free Software Foundation, either    //
//  version 3 of the License, or (at your option) any later version.                              //
//                                                                                                //
//  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     //
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     //
//  See the GNU Lesser General Public License for more details.                                   //
//                                                                                                //
//  You should have received a copy of the GNU Lesser General Public License along with this      //
//  library. If not, see <https://www.gnu.org/licenses/>.                                         //
//                                                                                                //
// ============================================================================================== //

#include "../convert.h"

#include <array>
#include <cassert>
#include <chrono>
#include <iostream>

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

auto main() -> int
{
    static constexpr long Iterations = 10000;

    static constexpr unsigned int Width = 1920;
    static constexpr unsigned int Height = 1080;

    std::vector<ifd::RgbaPixel> input(Width * Height);
    std::vector<ifd::BgraPixel> output(Width * Height);

    const auto start = std::chrono::steady_clock::now();

    for (long i = 0; i < Iterations; ++i)
        Convert::toBgra(input, output);

    const auto end = std::chrono::steady_clock::now();

    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Result: " << elapsed.count() / Iterations << " us" << std::endl;

    return 0;
}

// ---------------------------------------------------------------------------------------------- //
