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

#include <dlib/image_processing/frontal_face_detector.h>

IFD_BEGIN_NAMESPACE();

class DlibBackend : public Backend
{
public:
    static constexpr const char* Name = "Dlib";

public:
    DlibBackend(unsigned int width, unsigned int height);

    auto name() const -> std::string override;

    auto preferredImageFormat() const -> ImageFormat override;

    void process(std::span<const GrayscalePixel> image, RectList* results) const override;
    void process(std::span<const RgbPixel> image, RectList* results) const override;
    void process(std::span<const RgbaPixel> image, RectList* results) const override;
    void process(std::span<const BgrPixel> image, RectList* results) const override;
    void process(std::span<const BgraPixel> image, RectList* results) const override;

    static auto make(unsigned int width, unsigned int height) -> std::unique_ptr<Backend>;

private:
    void updateResults(RectList* results) const;

private:
    mutable dlib::frontal_face_detector m_detector;
    mutable std::vector<std::pair<double, dlib::rectangle>> m_detections;

    mutable dlib::array2d<unsigned char> m_grayscaleImage;
    mutable dlib::array2d<dlib::rgb_pixel> m_rgbImage;
};

IFD_END_NAMESPACE();
