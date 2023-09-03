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

#pragma once

#include "namespace.h"

#include <ifd.h>

IFD_BEGIN_NAMESPACE();

class Convert
{
public:
    static void toGrayscale(std::span<const RgbPixel> input, std::span<GrayscalePixel> output);
    static void toGrayscale(std::span<const RgbaPixel> input, std::span<GrayscalePixel> output);
    static void toGrayscale(std::span<const BgrPixel> input, std::span<GrayscalePixel> output);
    static void toGrayscale(std::span<const BgraPixel> input, std::span<GrayscalePixel> output);

    static void toRgb(std::span<const GrayscalePixel> input, std::span<RgbPixel> output);
    static void toRgb(std::span<const RgbaPixel> input, std::span<RgbPixel> output);
    static void toRgb(std::span<const BgrPixel> input, std::span<RgbPixel> output);
    static void toRgb(std::span<const BgraPixel> input, std::span<RgbPixel> output);

    static void toRgba(std::span<const GrayscalePixel> input, std::span<RgbaPixel> output);
    static void toRgba(std::span<const RgbPixel> input, std::span<RgbaPixel> output);
    static void toRgba(std::span<const BgrPixel> input, std::span<RgbaPixel> output);
    static void toRgba(std::span<const BgraPixel> input, std::span<RgbaPixel> output);

    static void toBgr(std::span<const GrayscalePixel> input, std::span<BgrPixel> output);
    static void toBgr(std::span<const RgbPixel> input, std::span<BgrPixel> output);
    static void toBgr(std::span<const RgbaPixel> input, std::span<BgrPixel> output);
    static void toBgr(std::span<const BgraPixel> input, std::span<BgrPixel> output);

    static void toBgra(std::span<const GrayscalePixel> input, std::span<BgraPixel> output);
    static void toBgra(std::span<const RgbPixel> input, std::span<BgraPixel> output);
    static void toBgra(std::span<const RgbaPixel> input, std::span<BgraPixel> output);
    static void toBgra(std::span<const BgrPixel> input, std::span<BgraPixel> output);
};

IFD_END_NAMESPACE();
