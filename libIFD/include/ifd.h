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

#ifdef _WIN32
  #ifdef IFD_BUILD_PROCESS
    #define IFD_EXPORT __declspec(dllexport)
  #else
    #define IFD_EXPORT __declspec(dllimport)
  #endif
#else
  #define IFD_EXPORT __attribute__((visibility("default")))
#endif

// ---------------------------------------------------------------------------------------------- //

#include <cstdint>
#include <memory>
#include <span>
#include <stdexcept>
#include <vector>

// ---------------------------------------------------------------------------------------------- //

namespace ifd {

// ---------------------------------------------------------------------------------------------- //

enum class ImageFormat
{
    Grayscale,
    Rgb,
    Rgba,
    Bgr,
    Bgra
};

using GrayscalePixel = uint8_t;

struct RgbPixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct RgbaPixel
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct BgrPixel
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

struct BgraPixel
{
    uint8_t b;
    uint8_t g;
    uint8_t r;
    uint8_t a;
};

static_assert(sizeof(RgbPixel) == 3);
static_assert(sizeof(RgbaPixel) == 4);
static_assert(sizeof(BgrPixel) == 3);
static_assert(sizeof(BgraPixel) == 4);

struct Rect
{
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
};

using RectList = std::vector<Rect>;
using Error = std::runtime_error;

// ---------------------------------------------------------------------------------------------- //

class IFD_EXPORT FaceDetector
{
public:
    FaceDetector(unsigned int width, unsigned int height,
                 const std::string& backend = getDefaultBackend());
    ~FaceDetector();

    FaceDetector(const FaceDetector&) = delete;
    FaceDetector(FaceDetector&&) = delete;

    auto operator=(const FaceDetector&) = delete;
    auto operator=(FaceDetector&&) = delete;

    auto width() const -> unsigned int;
    auto height() const -> unsigned int;

    auto backend() const -> std::string;

    auto preferredImageFormat() const -> ImageFormat;

    void process(std::span<const GrayscalePixel> image, RectList* results) const;
    void process(std::span<const RgbPixel> image, RectList* results) const;
    void process(std::span<const RgbaPixel> image, RectList* results) const;
    void process(std::span<const BgrPixel> image, RectList* results) const;
    void process(std::span<const BgraPixel> image, RectList* results) const;

    static auto getAvailableBackends() -> std::vector<std::string>;
    static auto getDefaultBackend() -> std::string;

private:
    class Private;
    std::unique_ptr<Private> d;
};

// ---------------------------------------------------------------------------------------------- //

} // End of namespace ifd

// ---------------------------------------------------------------------------------------------- //
