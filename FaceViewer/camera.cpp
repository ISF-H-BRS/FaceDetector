// ============================================================================================== //
//                                                                                                //
//  This file is part of the ISF Face Detector project.                                           //
//                                                                                                //
//  Author:                                                                                       //
//  Marcel Hasler <mahasler@gmail.com>                                                            //
//                                                                                                //
//  Copyright (c) 2021 - 2023                                                                     //
//  Bonn-Rhein-Sieg University of Applied Sciences                                                //
//                                                                                                //
//  This program is free software: you can redistribute it and/or modify it under the terms of    //
//  the GNU General Public License as published by the Free Software Foundation, either           //
//  version 3 of the License, or (at your option) any later version.                              //
//                                                                                                //
//  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;     //
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.     //
//  See the GNU General Public License for more details.                                          //
//                                                                                                //
//  You should have received a copy of the GNU General Public License along with this program.    //
//  If not, see <https://www.gnu.org/licenses/>.                                                  //
//                                                                                                //
// ============================================================================================== //

#include "camera.h"

#include <QThread>
#include <QTimer>

#include <openpnp-capture.h>

// ---------------------------------------------------------------------------------------------- //

namespace {
    using ContextGuard = std::unique_ptr<void, decltype(&Cap_releaseContext)>;

    auto fourccToString(uint32_t fourcc) -> QString
    {
        QString result;

        for (int i = 0; i < 4; ++i)
        {
            result += static_cast<char>(fourcc & 0xff);
            fourcc >>= 8;
        }

        return result;
    };

    auto formatToString(const CapFormatInfo& info) -> QString
    {
        return QString("%1 %2x%3 @ %4 FPS").arg(fourccToString(info.fourcc))
                                           .arg(info.width)
                                           .arg(info.height)
                                           .arg(info.fps);
    }
}

// ---------------------------------------------------------------------------------------------- //

class Camera::Private
{
public:
    ContextGuard context = { nullptr, Cap_releaseContext };

    size_t cameraIndex = 0;
    size_t formatIndex = 0;

    CapFormatInfo format;
    CapStream stream = -1;

    std::vector<uint8_t> frameData;

    QTimer timer;
};

// ---------------------------------------------------------------------------------------------- //

Camera::Camera(size_t cameraIndex, size_t formatIndex, QObject* parent)
    : QObject(parent),
      d(std::make_unique<Private>())
{
    d->context = ContextGuard(Cap_createContext(), Cap_releaseContext);
    d->cameraIndex = cameraIndex;
    d->formatIndex = formatIndex;

    CapResult result = Cap_getFormatInfo(d->context.get(), cameraIndex, formatIndex, &d->format);

    if (result != CAPRESULT_OK)
        throw Error("Unable to retrieve format info for selected camera.");

    d->frameData.resize(d->format.width * d->format.height * 3);

    d->timer.setSingleShot(true);
    d->timer.setInterval(1000 / d->format.fps);
    connect(&d->timer, SIGNAL(timeout()), this, SLOT(update()));
}

// ---------------------------------------------------------------------------------------------- //

Camera::~Camera()
{
    stop();
}

// ---------------------------------------------------------------------------------------------- //

auto Camera::formatString() const -> QString
{
    return formatToString(d->format);
}

// ---------------------------------------------------------------------------------------------- //

auto Camera::width() const -> quint32
{
    return d->format.width;
}

// ---------------------------------------------------------------------------------------------- //

auto Camera::height() const -> quint32
{
    return d->format.height;
}

// ---------------------------------------------------------------------------------------------- //

void Camera::start()
{
    if (d->stream >= 0)
        return;

    d->stream = Cap_openStream(d->context.get(), d->cameraIndex, d->formatIndex);

    if (d->stream < 0)
        throw Error("Unable to open camera stream.");

    d->timer.start();
}

// ---------------------------------------------------------------------------------------------- //

void Camera::stop()
{
    if (d->stream < 0)
        return;

    d->timer.stop();

    Cap_closeStream(d->context.get(), d->stream);
    d->stream = -1;
}

// ---------------------------------------------------------------------------------------------- //

void Camera::update()
{
    const bool haveFrame = waitForFrame();

    d->timer.start();

    if (!haveFrame)
        return;

    Cap_captureFrame(d->context.get(), d->stream, d->frameData.data(), d->frameData.size());

    QImage image(d->frameData.data(), d->format.width, d->format.height, QImage::Format_RGB888);
    emit frameAvailable(image.copy());
}

// ---------------------------------------------------------------------------------------------- //

auto Camera::waitForFrame() const -> bool
{
    for (int timeout = 0; timeout < 10; ++timeout)
    {
        if (Cap_hasNewFrame(d->context.get(), d->stream))
            return true;

        QThread::msleep(1);
    }

    return Cap_hasNewFrame(d->context.get(), d->stream);
}

// ---------------------------------------------------------------------------------------------- //

auto Camera::getAvailableCameras() -> std::vector<CameraInfo>
{
    std::vector<CameraInfo> cameras;

    ContextGuard guard(Cap_createContext(), Cap_releaseContext);
    CapContext context = guard.get();

    const uint32_t count = Cap_getDeviceCount(context);

    for (uint32_t i = 0; i < count; ++i)
    {
        const char* uniqueName = Cap_getDeviceUniqueID(context, i);
        const char* displayName = Cap_getDeviceName(context, i);

        const int32_t numFormats = Cap_getNumFormats(context, i);

        if (numFormats < 0)
            continue;

        std::vector<QString> formats;

        for (int32_t j = 0; j < numFormats; ++j)
        {
            CapFormatInfo info;
            CapResult result = Cap_getFormatInfo(context, i, j, &info);

            if (result != CAPRESULT_OK)
                continue;

            formats.push_back(formatToString(info));
        }

        cameras.emplace_back(uniqueName, displayName, std::move(formats));
    }

    return cameras;
}

// ---------------------------------------------------------------------------------------------- //
