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

#ifdef IFD_USE_DLIB
#include "dlibbackend.h"
#endif
#ifdef IFD_USE_LIBFACEDETECTION
#include "libfacedetectionbackend.h"
#endif
#ifdef IFD_USE_MEDIAPIPE
#include "mediapipebackend.h"
#endif
#ifdef IFD_USE_OPENCV
#include "opencvbackend.h"
#endif

#include "dummybackend.h"

#include <ifd.h>

#include <functional>
#include <mutex>

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

namespace {
    using Factory = std::function<std::unique_ptr<Backend>(unsigned int,unsigned int)>;
    using FactoryList = std::vector<std::pair<const char*, Factory>>;
}

// ---------------------------------------------------------------------------------------------- //

class FaceDetector::Private
{
public:
    std::unique_ptr<Backend> backend;
    std::mutex mutex;

    static auto getFactories() -> const FactoryList&;
};

// ---------------------------------------------------------------------------------------------- //

FaceDetector::FaceDetector(unsigned int width, unsigned int height, const std::string& backend)
    : d(std::make_unique<Private>())
{
    const FactoryList& factories = Private::getFactories();

    for (const auto& factory : factories)
    {
        if (factory.first == backend)
        {
            d->backend = factory.second(width, height);
            break;
        }
    }

    if (!d->backend)
        throw Error("Unsupported backend \"" + backend + "\" requested.");
}

// ---------------------------------------------------------------------------------------------- //

FaceDetector::~FaceDetector() = default;

// ---------------------------------------------------------------------------------------------- //

auto FaceDetector::width() const -> unsigned int
{
    return d->backend->width();
}

// ---------------------------------------------------------------------------------------------- //

auto FaceDetector::height() const -> unsigned int
{
    return d->backend->height();
}

// ---------------------------------------------------------------------------------------------- //

auto FaceDetector::backend() const -> std::string
{
    return d->backend->name();
}

// ---------------------------------------------------------------------------------------------- //

auto FaceDetector::preferredImageFormat() const -> ImageFormat
{
    return d->backend->preferredImageFormat();
}

// ---------------------------------------------------------------------------------------------- //

void FaceDetector::process(std::span<const GrayscalePixel> image, RectList* results) const
{
    std::lock_guard lock(d->mutex);
    d->backend->process(image, results);
}

// ---------------------------------------------------------------------------------------------- //

void FaceDetector::process(std::span<const RgbPixel> image, RectList* results) const
{
    std::lock_guard lock(d->mutex);
    d->backend->process(image, results);
}

// ---------------------------------------------------------------------------------------------- //

void FaceDetector::process(std::span<const RgbaPixel> image, RectList* results) const
{
    std::lock_guard lock(d->mutex);
    d->backend->process(image, results);
}

// ---------------------------------------------------------------------------------------------- //

void FaceDetector::process(std::span<const BgrPixel> image, RectList* results) const
{
    std::lock_guard lock(d->mutex);
    d->backend->process(image, results);
}

// ---------------------------------------------------------------------------------------------- //

void FaceDetector::process(std::span<const BgraPixel> image, RectList* results) const
{
    std::lock_guard lock(d->mutex);
    d->backend->process(image, results);
}

// ---------------------------------------------------------------------------------------------- //

auto FaceDetector::getAvailableBackends() -> std::vector<std::string>
{
    const FactoryList& factories = Private::getFactories();

    std::vector<std::string> backends;

    for (const auto& factory : factories)
        backends.emplace_back(factory.first);

    return backends;
}

// ---------------------------------------------------------------------------------------------- //

auto FaceDetector::getDefaultBackend() -> std::string
{
    const FactoryList& factories = Private::getFactories();
    return factories.at(0).first;
}

// ---------------------------------------------------------------------------------------------- //

auto FaceDetector::Private::getFactories() -> const FactoryList&
{
    static const FactoryList factories = {
#ifdef IFD_USE_LIBFACEDETECTION
        { LibFaceDetectionBackend::Name, LibFaceDetectionBackend::make },
#endif
#ifdef IFD_USE_MEDIAPIPE
        { MediaPipeBackend::Name, MediaPipeBackend::make },
#endif
#ifdef IFD_USE_OPENCV
        { OpenCVBackend::Name, OpenCVBackend::make },
#endif
#ifdef IFD_USE_DLIB
        { DlibBackend::Name, DlibBackend::make },
#endif
        { DummyBackend::Name, DummyBackend::make }
    };

    return factories;
}

// ---------------------------------------------------------------------------------------------- //
