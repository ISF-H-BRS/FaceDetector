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

#include "dummybackend.h"

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

DummyBackend::DummyBackend(unsigned int width, unsigned int height)
    : Backend(width, height)
{
}

// ---------------------------------------------------------------------------------------------- //

auto DummyBackend::name() const -> std::string
{
    return Name;
}

// ---------------------------------------------------------------------------------------------- //

auto DummyBackend::preferredImageFormat() const -> ImageFormat
{
    return ImageFormat::Grayscale;
}

// ---------------------------------------------------------------------------------------------- //

void DummyBackend::process(std::span<const GrayscalePixel>, RectList* results) const
{
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

void DummyBackend::process(std::span<const RgbPixel>, RectList* results) const
{
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

void DummyBackend::process(std::span<const RgbaPixel>, RectList* results) const
{
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

void DummyBackend::process(std::span<const BgrPixel>, RectList* results) const
{
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

void DummyBackend::process(std::span<const BgraPixel>, RectList* results) const
{
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

auto DummyBackend::make(unsigned int width, unsigned int height) -> std::unique_ptr<Backend>
{
    return std::make_unique<DummyBackend>(width, height);
}

// ---------------------------------------------------------------------------------------------- //

void DummyBackend::updateResults(RectList* results) const
{
    const unsigned int w = width() / 8;
    const unsigned int h = height() / 4;
    const unsigned int x = (width() / 2) - (w / 2);
    const unsigned int y = (height() / 2) - (h / 2);

    results->clear();
    results->emplace_back(x, y, w, h);
}

// ---------------------------------------------------------------------------------------------- //
