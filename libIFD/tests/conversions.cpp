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
#include <iostream>

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

namespace {
    constexpr GrayscalePixel GrayscaleRed   = 76;
    constexpr GrayscalePixel GrayscaleGreen = 149;
    constexpr GrayscalePixel GrayscaleBlue  = 29;
    constexpr GrayscalePixel GrayscaleGray  = 128;

    constexpr RgbPixel RgbRed     = { 255,   0,   0 };
    constexpr RgbPixel RgbGreen   = {   0, 255,   0 };
    constexpr RgbPixel RgbBlue    = {   0,   0, 255 };
    constexpr RgbPixel RgbGray    = { 128, 128, 128 };

    constexpr RgbaPixel RgbaRed   = { 255,   0,   0, 255 };
    constexpr RgbaPixel RgbaGreen = {   0, 255,   0, 255 };
    constexpr RgbaPixel RgbaBlue  = {   0,   0, 255, 255 };
    constexpr RgbaPixel RgbaGray  = { 128, 128, 128, 255 };

    constexpr BgrPixel BgrRed     = {   0,   0, 255 };
    constexpr BgrPixel BgrGreen   = {   0, 255,   0 };
    constexpr BgrPixel BgrBlue    = { 255,   0,   0 };
    constexpr BgrPixel BgrGray    = { 128, 128, 128 };

    constexpr BgraPixel BgraRed   = {   0,   0, 255, 255 };
    constexpr BgraPixel BgraGreen = {   0, 255,   0, 255 };
    constexpr BgraPixel BgraBlue  = { 255,   0,   0, 255 };
    constexpr BgraPixel BgraGray  = { 128, 128, 128, 255 };

    auto operator==(const RgbPixel& p1, const RgbPixel& p2)
    {
        return p1.r == p2.r && p1.g == p2.g && p1.b == p2.b;
    }

    auto operator==(const RgbaPixel& p1, const RgbaPixel& p2)
    {
        return p1.r == p2.r && p1.g == p2.g && p1.b == p2.b && p1.a == p2.a;
    }

    auto operator==(const BgrPixel& p1, const BgrPixel& p2)
    {
        return p1.b == p2.b && p1.g == p2.g && p1.r == p2.r;
    }

    auto operator==(const BgraPixel& p1, const BgraPixel& p2)
    {
        return p1.b == p2.b && p1.g == p2.g && p1.r == p2.r && p1.a == p2.a;
    }
}

// ---------------------------------------------------------------------------------------------- //

template <typename InType, typename OutType>
void test(InType in, OutType out,
          void(*func)(std::span<const InType>, std::span<OutType>))
{
    std::array<InType, 1> inData = { in };
    std::array<OutType, 1> outData;

    func(inData, outData);
    assert(outData[0] == out);
}

// ---------------------------------------------------------------------------------------------- //

auto main() -> int
{
    // Grayscale
    test(RgbRed,    GrayscaleRed,   &Convert::toGrayscale);
    test(RgbGreen,  GrayscaleGreen, &Convert::toGrayscale);
    test(RgbBlue,   GrayscaleBlue,  &Convert::toGrayscale);

    test(RgbaRed,   GrayscaleRed,   &Convert::toGrayscale);
    test(RgbaGreen, GrayscaleGreen, &Convert::toGrayscale);
    test(RgbaBlue,  GrayscaleBlue,  &Convert::toGrayscale);

    test(BgrRed,    GrayscaleRed,   &Convert::toGrayscale);
    test(BgrGreen,  GrayscaleGreen, &Convert::toGrayscale);
    test(BgrBlue,   GrayscaleBlue,  &Convert::toGrayscale);

    test(BgraRed,   GrayscaleRed,   &Convert::toGrayscale);
    test(BgraGreen, GrayscaleGreen, &Convert::toGrayscale);
    test(BgraBlue,  GrayscaleBlue,  &Convert::toGrayscale);

    // RGB
    test(GrayscaleGray, RgbGray, &Convert::toRgb);

    test(RgbaRed,   RgbRed,   &Convert::toRgb);
    test(RgbaGreen, RgbGreen, &Convert::toRgb);
    test(RgbaBlue,  RgbBlue,  &Convert::toRgb);

    test(BgrRed,    RgbRed,   &Convert::toRgb);
    test(BgrGreen,  RgbGreen, &Convert::toRgb);
    test(BgrBlue,   RgbBlue,  &Convert::toRgb);

    test(BgraRed,   RgbRed,   &Convert::toRgb);
    test(BgraGreen, RgbGreen, &Convert::toRgb);
    test(BgraBlue,  RgbBlue,  &Convert::toRgb);

    // RGBA
    test(GrayscaleGray, RgbaGray, &Convert::toRgba);

    test(RgbRed,    RgbaRed,   &Convert::toRgba);
    test(RgbGreen,  RgbaGreen, &Convert::toRgba);
    test(RgbBlue,   RgbaBlue,  &Convert::toRgba);

    test(BgrRed,    RgbaRed,   &Convert::toRgba);
    test(BgrGreen,  RgbaGreen, &Convert::toRgba);
    test(BgrBlue,   RgbaBlue,  &Convert::toRgba);

    test(BgraRed,   RgbaRed,   &Convert::toRgba);
    test(BgraGreen, RgbaGreen, &Convert::toRgba);
    test(BgraBlue,  RgbaBlue,  &Convert::toRgba);

    // BGR
    test(GrayscaleGray, BgrGray, &Convert::toBgr);

    test(RgbRed,    BgrRed,   &Convert::toBgr);
    test(RgbGreen,  BgrGreen, &Convert::toBgr);
    test(RgbBlue,   BgrBlue,  &Convert::toBgr);

    test(RgbaRed,   BgrRed,   &Convert::toBgr);
    test(RgbaGreen, BgrGreen, &Convert::toBgr);
    test(RgbaBlue,  BgrBlue,  &Convert::toBgr);

    test(BgraRed,   BgrRed,   &Convert::toBgr);
    test(BgraGreen, BgrGreen, &Convert::toBgr);
    test(BgraBlue,  BgrBlue,  &Convert::toBgr);

    // BGRA
    test(GrayscaleGray, BgraGray, &Convert::toBgra);

    test(RgbRed,    BgraRed,   &Convert::toBgra);
    test(RgbGreen,  BgraGreen, &Convert::toBgra);
    test(RgbBlue,   BgraBlue,  &Convert::toBgra);

    test(RgbaRed,   BgraRed,   &Convert::toBgra);
    test(RgbaGreen, BgraGreen, &Convert::toBgra);
    test(RgbaBlue,  BgraBlue,  &Convert::toBgra);

    test(BgrRed,    BgraRed,   &Convert::toBgra);
    test(BgrGreen,  BgraGreen, &Convert::toBgra);
    test(BgrBlue,   BgraBlue,  &Convert::toBgra);

    std::cout << "All tests passed." << std::endl;
    return 0;
}

// ---------------------------------------------------------------------------------------------- //
