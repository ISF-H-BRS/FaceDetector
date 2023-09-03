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

#include "convert.h"

#include <algorithm>
#include <execution>

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

namespace {
    auto rgbToY(uint8_t red, uint8_t green, uint8_t blue) -> uint8_t
    {
        const auto r = static_cast<double>(red);
        const auto g = static_cast<double>(green);
        const auto b = static_cast<double>(blue);

        return static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
    }

    constexpr auto ExecutionPolicy = std::execution::par_unseq;
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toGrayscale(std::span<const RgbPixel> input, std::span<GrayscalePixel> output)
{
    static const auto convert = [](const RgbPixel& input) -> GrayscalePixel {
        return ::rgbToY(input.r, input.g, input.b);
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toGrayscale(std::span<const RgbaPixel> input, std::span<GrayscalePixel> output)
{
    static const auto convert = [](const RgbaPixel& input) -> GrayscalePixel {
        return ::rgbToY(input.r, input.g, input.b);
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toGrayscale(std::span<const BgrPixel> input, std::span<GrayscalePixel> output)
{
    static const auto convert = [](const BgrPixel& input) -> GrayscalePixel {
        return ::rgbToY(input.r, input.g, input.b);
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toGrayscale(std::span<const BgraPixel> input, std::span<GrayscalePixel> output)
{
    static const auto convert = [](const BgraPixel& input) -> GrayscalePixel {
        return ::rgbToY(input.r, input.g, input.b);
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toRgb(std::span<const GrayscalePixel> input, std::span<RgbPixel> output)
{
    static const auto convert = [](const GrayscalePixel& input) -> RgbPixel {
        return { input, input, input };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toRgb(std::span<const RgbaPixel> input, std::span<RgbPixel> output)
{
    static const auto convert = [](const RgbaPixel& input) -> RgbPixel {
        return { input.r, input.g, input.b };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toRgb(std::span<const BgrPixel> input, std::span<RgbPixel> output)
{
    static const auto convert = [](const BgrPixel& input) -> RgbPixel {
        return { input.r, input.g, input.b };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toRgb(std::span<const BgraPixel> input, std::span<RgbPixel> output)
{
    static const auto convert = [](const BgraPixel& input) -> RgbPixel {
        return { input.r, input.g, input.b };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toRgba(std::span<const GrayscalePixel> input, std::span<RgbaPixel> output)
{
    static const auto convert = [](const GrayscalePixel& input) -> RgbaPixel {
        return { input, input, input, 255 };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toRgba(std::span<const RgbPixel> input, std::span<RgbaPixel> output)
{
    static const auto convert = [](const RgbPixel& input) -> RgbaPixel {
        return { input.r, input.g, input.b, 255 };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toRgba(std::span<const BgrPixel> input, std::span<RgbaPixel> output)
{
    static const auto convert = [](const BgrPixel& input) -> RgbaPixel {
        return { input.r, input.g, input.b, 255 };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toRgba(std::span<const BgraPixel> input, std::span<RgbaPixel> output)
{
    static const auto convert = [](const BgraPixel& input) -> RgbaPixel {
        return { input.r, input.g, input.b, input.a };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toBgr(std::span<const GrayscalePixel> input, std::span<BgrPixel> output)
{
    static const auto convert = [](const GrayscalePixel& input) -> BgrPixel {
        return { input, input, input };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toBgr(std::span<const RgbPixel> input, std::span<BgrPixel> output)
{
    static const auto convert = [](const RgbPixel& input) -> BgrPixel {
        return { input.b, input.g, input.r };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toBgr(std::span<const RgbaPixel> input, std::span<BgrPixel> output)
{
    static const auto convert = [](const RgbaPixel& input) -> BgrPixel {
        return { input.b, input.g, input.r };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toBgr(std::span<const BgraPixel> input, std::span<BgrPixel> output)
{
    static const auto convert = [](const BgraPixel& input) -> BgrPixel {
        return { input.b, input.g, input.r };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toBgra(std::span<const GrayscalePixel> input, std::span<BgraPixel> output)
{
    static const auto convert = [](const GrayscalePixel& input) -> BgraPixel {
        return { input, input, input, 255 };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toBgra(std::span<const RgbPixel> input, std::span<BgraPixel> output)
{
    static const auto convert = [](const RgbPixel& input) -> BgraPixel {
        return { input.b, input.g, input.r, 255 };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toBgra(std::span<const RgbaPixel> input, std::span<BgraPixel> output)
{
    static const auto convert = [](const RgbaPixel& input) -> BgraPixel {
        return { input.b, input.g, input.r, input.a };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //

void Convert::toBgra(std::span<const BgrPixel> input, std::span<BgraPixel> output)
{
    static const auto convert = [](const BgrPixel& input) -> BgraPixel {
        return { input.b, input.g, input.r, 255 };
    };

    std::transform(ExecutionPolicy, input.begin(), input.end(), output.begin(), convert);
}

// ---------------------------------------------------------------------------------------------- //
