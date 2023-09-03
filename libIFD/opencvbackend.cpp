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
#include "opencvbackend.h"
#include "opencvmodel.h" // auto-generated

#include <chrono>
#include <iostream>

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

OpenCVBackend::OpenCVBackend(unsigned int width, unsigned int height)
    : Backend(width, height),
      m_cvImage(height, width, CV_8UC1),
      m_grayscaleImage(width * height)
{
    cv::FileStorage fs(opencvmodel, cv::FileStorage::READ | cv::FileStorage::MEMORY);

    if (!m_classifier.read(fs.getFirstTopLevelNode()))
        throw Error("Unable to load model.");
}

// ---------------------------------------------------------------------------------------------- //

auto OpenCVBackend::name() const -> std::string
{
    return Name;
}

// ---------------------------------------------------------------------------------------------- //

auto OpenCVBackend::preferredImageFormat() const -> ImageFormat
{
    return ImageFormat::Grayscale;
}

// ---------------------------------------------------------------------------------------------- //

void OpenCVBackend::process(std::span<const GrayscalePixel> image, RectList* results) const
{
    auto data = const_cast<unsigned char*>(image.data());
    const cv::Mat cvImage(height(), width(), CV_8UC1, data);

    m_classifier.detectMultiScale(cvImage, m_rects);
    updateResults(results);
}

// ---------------------------------------------------------------------------------------------- //

void OpenCVBackend::process(std::span<const RgbPixel> image, RectList* results) const
{
    Convert::toGrayscale(image, m_grayscaleImage);
    process(m_grayscaleImage, results);
}

// ---------------------------------------------------------------------------------------------- //

void OpenCVBackend::process(std::span<const RgbaPixel> image, RectList* results) const
{
    Convert::toGrayscale(image, m_grayscaleImage);
    process(m_grayscaleImage, results);
}

// ---------------------------------------------------------------------------------------------- //

void OpenCVBackend::process(std::span<const BgrPixel> image, RectList* results) const
{
    Convert::toGrayscale(image, m_grayscaleImage);
    process(m_grayscaleImage, results);
}

// ---------------------------------------------------------------------------------------------- //

void OpenCVBackend::process(std::span<const BgraPixel> image, RectList* results) const
{
    Convert::toGrayscale(image, m_grayscaleImage);
    process(m_grayscaleImage, results);
}

// ---------------------------------------------------------------------------------------------- //

void OpenCVBackend::updateResults(RectList* results) const
{
    results->clear();

    for (const auto& rect : m_rects)
        results->emplace_back(rect.x, rect.y, rect.width, rect.height);
}

// ---------------------------------------------------------------------------------------------- //

auto OpenCVBackend::make(unsigned int width, unsigned int height) -> std::unique_ptr<Backend>
{
    return std::make_unique<OpenCVBackend>(width, height);
}

// ---------------------------------------------------------------------------------------------- //
