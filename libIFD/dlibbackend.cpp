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

#include "dlibbackend.h"

#ifdef __cpuid
#undef __cpuid // Work-around for mismatch in msys headers
#endif

#include <algorithm>
#include <execution>

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

namespace {
    template <typename T>
    auto toDlibPixel(const T& pixel) -> dlib::rgb_pixel
    {
        return { pixel.r, pixel.g, pixel.b };
    };

    constexpr auto ExecutionPolicy = std::execution::par_unseq;
}

// ---------------------------------------------------------------------------------------------- //

DlibBackend::DlibBackend(unsigned int width, unsigned int height)
    : Backend(width, height),
      m_detector(dlib::get_frontal_face_detector()),
      m_grayscaleImage(height, width),
      m_rgbImage(height, width)
{
}

// ---------------------------------------------------------------------------------------------- //

auto DlibBackend::name() const -> std::string
{
    return Name;
}

// ---------------------------------------------------------------------------------------------- //

auto DlibBackend::preferredImageFormat() const -> ImageFormat
{
    return ImageFormat::Bgr;
}

// ---------------------------------------------------------------------------------------------- //

void DlibBackend::process(std::span<const GrayscalePixel> image, RectList* results) const
{
    std::copy(image.begin(), image.end(), m_grayscaleImage.begin());

    m_detector(m_grayscaleImage, m_detections);
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

void DlibBackend::process(std::span<const RgbPixel> image, RectList* results) const
{
    std::transform(ExecutionPolicy,
                   image.begin(), image.end(), m_rgbImage.begin(), toDlibPixel<RgbPixel>);

    m_detector(m_rgbImage, m_detections);
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

void DlibBackend::process(std::span<const RgbaPixel> image, RectList* results) const
{
    std::transform(ExecutionPolicy,
                   image.begin(), image.end(), m_rgbImage.begin(), toDlibPixel<RgbaPixel>);

    m_detector(m_rgbImage, m_detections);
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

void DlibBackend::process(std::span<const BgrPixel> image, RectList* results) const
{
    std::transform(ExecutionPolicy,
                   image.begin(), image.end(), m_rgbImage.begin(), toDlibPixel<BgrPixel>);

    m_detector(m_rgbImage, m_detections);
    updateResults(results);

}

// ---------------------------------------------------------------------------------------------- //

void DlibBackend::process(std::span<const BgraPixel> image, RectList* results) const
{
    std::transform(ExecutionPolicy,
                   image.begin(), image.end(), m_rgbImage.begin(), toDlibPixel<BgraPixel>);

    m_detector(m_rgbImage, m_detections);
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

void DlibBackend::updateResults(RectList* results) const
{
    static constexpr double MinimumConfidence = 0.1;

    results->clear();

    for (const auto& det : m_detections)
    {
        const double confidence = det.first;
        const dlib::rectangle& rect = det.second;

        if (confidence >= MinimumConfidence)
            results->emplace_back(rect.left(), rect.top(), rect.width(), rect.height());
    }
}

// ---------------------------------------------------------------------------------------------- //

auto DlibBackend::make(unsigned int width, unsigned int height) -> std::unique_ptr<Backend>
{
    return std::make_unique<DlibBackend>(width, height);
}

// ---------------------------------------------------------------------------------------------- //
