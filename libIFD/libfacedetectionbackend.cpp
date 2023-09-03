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
#include "libfacedetectionbackend.h"

#include <facedetectcnn.h>

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

namespace {
    static constexpr size_t BufferSize = 0x20000;
}

// ---------------------------------------------------------------------------------------------- //

LibFaceDetectionBackend::LibFaceDetectionBackend(unsigned int width, unsigned int height)
    : Backend(width, height),
      m_buffer(BufferSize),
      m_image(width * height)
{
}

// ---------------------------------------------------------------------------------------------- //

auto LibFaceDetectionBackend::name() const -> std::string
{
    return Name;
}

// ---------------------------------------------------------------------------------------------- //

auto LibFaceDetectionBackend::preferredImageFormat() const -> ImageFormat
{
    return ImageFormat::Bgr;
}

// ---------------------------------------------------------------------------------------------- //

void LibFaceDetectionBackend::process(std::span<const GrayscalePixel> image,
                                      RectList* results) const
{
    Convert::toBgr(image, m_image);
    process(m_image, results);
}

// ---------------------------------------------------------------------------------------------- //

void LibFaceDetectionBackend::process(std::span<const RgbPixel> image, RectList* results) const
{
    Convert::toBgr(image, m_image);
    process(m_image, results);
}

// ---------------------------------------------------------------------------------------------- //

void LibFaceDetectionBackend::process(std::span<const RgbaPixel> image, RectList* results) const
{
    Convert::toBgr(image, m_image);
    process(m_image, results);
}

// ---------------------------------------------------------------------------------------------- //

void LibFaceDetectionBackend::process(std::span<const BgrPixel> image, RectList* results) const
{
    static constexpr int MinimumConfidence = 50;
    static constexpr int RecordsStride = 142;

    results->clear();

    auto data = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(image.data()));

    const unsigned int width = this->width();
    const unsigned int height = this->height();

    int* ptr = facedetect_cnn(m_buffer.data(), data, width, height, width * sizeof(BgrPixel));

    if (!ptr)
        return;

    const int count = *ptr;
    const auto records = reinterpret_cast<short*>(ptr + 1);

    for (int i = 0; i < count; ++i)
    {
        const auto record = records + i*RecordsStride;
        const auto confidence = record[0];

        if (confidence > MinimumConfidence)
        {
            const auto x = static_cast<unsigned int>(record[1]);
            const auto y = static_cast<unsigned int>(record[2]);
            const auto w = static_cast<unsigned int>(record[3]);
            const auto h = static_cast<unsigned int>(record[4]);

            results->emplace_back(x, y, w, h);
        }
    }
}

// ---------------------------------------------------------------------------------------------- //

void LibFaceDetectionBackend::process(std::span<const BgraPixel> image, RectList* results) const
{
    Convert::toBgr(image, m_image);
    process(m_image, results);
}

// ---------------------------------------------------------------------------------------------- //

auto LibFaceDetectionBackend::make(unsigned int width,
                                   unsigned int height) -> std::unique_ptr<Backend>
{
    return std::make_unique<LibFaceDetectionBackend>(width, height);
}

// ---------------------------------------------------------------------------------------------- //
