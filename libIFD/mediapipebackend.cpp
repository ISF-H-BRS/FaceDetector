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
#include "mediapipebackend.h"

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

MediaPipeBackend::MediaPipeBackend(unsigned int width, unsigned int height)
    : Backend(width, height),
      m_impl(width, height),
      m_rgbaImage(width * height)
{
}

// ---------------------------------------------------------------------------------------------- //

auto MediaPipeBackend::name() const -> std::string
{
    return Name;
}

// ---------------------------------------------------------------------------------------------- //

auto MediaPipeBackend::preferredImageFormat() const -> ImageFormat
{
    return ImageFormat::Rgba;
}

// ---------------------------------------------------------------------------------------------- //

void MediaPipeBackend::process(std::span<const GrayscalePixel> image, RectList* results) const
{
    Convert::toRgba(image, m_rgbaImage);
    process(m_rgbaImage, results);
}

// ---------------------------------------------------------------------------------------------- //

void MediaPipeBackend::process(std::span<const RgbPixel> image, RectList* results) const
{
    Convert::toRgba(image, m_rgbaImage);
    process(m_rgbaImage, results);
}

// ---------------------------------------------------------------------------------------------- //

void MediaPipeBackend::process(std::span<const RgbaPixel> image, RectList* results) const
{
    m_impl.process(image, results);
}

// ---------------------------------------------------------------------------------------------- //

void MediaPipeBackend::process(std::span<const BgrPixel> image, RectList* results) const
{
    Convert::toRgba(image, m_rgbaImage);
    process(m_rgbaImage, results);
}

// ---------------------------------------------------------------------------------------------- //

void MediaPipeBackend::process(std::span<const BgraPixel> image, RectList* results) const
{
    Convert::toRgba(image, m_rgbaImage);
    process(m_rgbaImage, results);
}

// ---------------------------------------------------------------------------------------------- //

auto MediaPipeBackend::make(unsigned int width, unsigned int height) -> std::unique_ptr<Backend>
{
    return std::make_unique<MediaPipeBackend>(width, height);
}

// ---------------------------------------------------------------------------------------------- //
