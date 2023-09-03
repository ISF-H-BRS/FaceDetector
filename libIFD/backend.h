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

class Backend
{
public:
    Backend(unsigned int width, unsigned int height)
        : m_width(width), m_height(height) {}

    virtual ~Backend() = default;

    Backend(const Backend&) = delete;
    Backend(Backend&&) = delete;

    auto operator=(const Backend&) = delete;
    auto operator=(Backend&&) = delete;

    auto width() const -> unsigned int { return m_width; }
    auto height() const -> unsigned int { return m_height; }

    virtual auto name() const -> std::string = 0;

    virtual auto preferredImageFormat() const -> ImageFormat = 0;

    virtual void process(std::span<const GrayscalePixel> image, RectList* results) const = 0;
    virtual void process(std::span<const RgbPixel> image, RectList* results) const = 0;
    virtual void process(std::span<const RgbaPixel> image, RectList* results) const = 0;
    virtual void process(std::span<const BgrPixel> image, RectList* results) const = 0;
    virtual void process(std::span<const BgraPixel> image, RectList* results) const = 0;

private:
    unsigned int m_width;
    unsigned int m_height;
};

IFD_END_NAMESPACE();
