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

#include "backend.h"
#include "mediapipe/mediapipebackendimpl.h"

IFD_BEGIN_NAMESPACE();

class MediaPipeBackend : public Backend
{
public:
    static constexpr const char* Name = "MediaPipe";

public:
    MediaPipeBackend(unsigned int width, unsigned int height);

    auto name() const -> std::string override;

    auto preferredImageFormat() const -> ImageFormat override;

    void process(std::span<const GrayscalePixel> image, RectList* results) const override;
    void process(std::span<const RgbPixel> image, RectList* results) const override;
    void process(std::span<const RgbaPixel> image, RectList* results) const override;
    void process(std::span<const BgrPixel> image, RectList* results) const override;
    void process(std::span<const BgraPixel> image, RectList* results) const override;

    static auto make(unsigned int width, unsigned int height) -> std::unique_ptr<Backend>;

private:
    MediaPipeBackendImpl m_impl;
    mutable std::vector<RgbaPixel> m_rgbaImage;
};

IFD_END_NAMESPACE();
