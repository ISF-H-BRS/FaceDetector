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

#include "mediapipebackendimpl.h"

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/port/parse_text_proto.h"

#ifndef MEDIAPIPE_DISABLE_GPU
#include "mediapipe/gpu/gl_calculator_helper.h"
#include "mediapipe/gpu/gpu_buffer.h"
#include "mediapipe/gpu/gpu_shared_data_internal.h"
#endif

// ---------------------------------------------------------------------------------------------- //

using namespace ifd;

// ---------------------------------------------------------------------------------------------- //

namespace {
    constexpr const char* InputStream = "input_video";
    constexpr const char* PresenceStream = "face_presence";
    constexpr const char* DetectionStream = "face_detections";

    constexpr const char* GraphText = R"(
        input_stream: "input_video"

        output_stream: "face_detections"
        output_stream: "face_presence"

        node {
    )"
#ifdef MEDIAPIPE_DISABLE_GPU
    R"(
            calculator: "FaceDetectionFullRangeCpu"
    )"
#else
    R"(
            calculator: "FaceDetectionFullRangeGpu"
    )"
#endif
    R"(
            input_stream: "IMAGE:input_video"
            output_stream: "DETECTIONS:face_detections"
        }

        node {
            calculator: "PacketPresenceCalculator"
            input_stream: "PACKET:face_detections"
            output_stream: "PRESENCE:face_presence"
        }
    )";
}

// ---------------------------------------------------------------------------------------------- //

class MediaPipeBackendImpl::Private
{
public:
    mediapipe::CalculatorGraph graph;

#ifndef MEDIAPIPE_DISABLE_GPU
    mediapipe::GlCalculatorHelper gpuHelper;
#endif

    mediapipe::StatusOrPoller presencePoller;
    mediapipe::StatusOrPoller detectionPoller;

    mediapipe::Packet presencePacket;
    mediapipe::Packet detectionPacket;
};

// ---------------------------------------------------------------------------------------------- //

auto toString(const absl::Status& status) -> std::string
{
    return status.message().data();
}

// ---------------------------------------------------------------------------------------------- //

template <typename T>
auto toString(const absl::StatusOr<T>& status) -> std::string
{
    return toString(status.status());
}

// ---------------------------------------------------------------------------------------------- //

MediaPipeBackendImpl::MediaPipeBackendImpl(unsigned int width, unsigned int height)
    : d(std::make_unique<Private>()),
      m_width(width),
      m_height(height)
{
    mediapipe::CalculatorGraphConfig config;

    if (!mediapipe::ParseTextProto(GraphText, &config))
        throw Error("Unable to parse graph text.");

    absl::Status initialized = d->graph.Initialize(config);

    if (!initialized.ok())
        throw Error("Unable to initialize graph: " + toString(initialized));

#ifndef MEDIAPIPE_DISABLE_GPU
    auto resources = mediapipe::GpuResources::Create();

    if (!resources.ok())
        throw Error("Unable to create GPU resources: " + toString(resources));

    absl::Status resourcesSet = d->graph.SetGpuResources(std::move(*resources));

    if (!resourcesSet.ok())
        throw Error("Unable to set GPU resources: " + toString(resourcesSet));

    d->gpuHelper.InitializeForTest(d->graph.GetGpuResources().get());
#endif

    d->presencePoller = d->graph.AddOutputStreamPoller(PresenceStream);

    if (!d->presencePoller.ok())
        throw Error("Unable to add output-stream poller: " + toString(d->presencePoller));

    d->detectionPoller = d->graph.AddOutputStreamPoller(DetectionStream);

    if (!d->detectionPoller.ok())
        throw Error("Unable to add output-stream poller: " + toString(d->detectionPoller));

    absl::Status started = d->graph.StartRun({});

    if (!started.ok())
        throw Error("Unable to start graph: " + toString(started));
}

// ---------------------------------------------------------------------------------------------- //

MediaPipeBackendImpl::~MediaPipeBackendImpl()
{
    d->graph.CloseInputStream(InputStream);
    d->graph.WaitUntilDone();
}

// ---------------------------------------------------------------------------------------------- //

void MediaPipeBackendImpl::process(std::span<const RgbaPixel> image, RectList* results) const
{
    static constexpr auto ImageFormat = mediapipe::ImageFormat::SRGBA;
    static constexpr size_t ChannelCount = 4;

    static const auto deleter = [](uint8_t*) {}; // Don't allow image frame to delete data

    static const auto toUint = [](auto value) {
        return static_cast<unsigned int>(std::round(value));
    };

    const size_t stepWidth = m_width * ChannelCount;
    auto ptr = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(image.data()));

    auto frame = std::make_unique<mediapipe::ImageFrame>(ImageFormat, m_width, m_height,
                                                         stepWidth, ptr, deleter);
    const mediapipe::Timestamp timestamp(m_timestamp++);

#ifdef MEDIAPIPE_DISABLE_GPU
    auto packet = mediapipe::Adopt(frame.release());
    absl::Status added = d->graph.AddPacketToInputStream(InputStream, packet.At(timestamp));
#else
    const auto addPacket = [&]() -> absl::Status
    {
        auto buffer = d->gpuHelper.GpuBufferCopyingImageFrame(*frame.get());
        auto texture = d->gpuHelper.CreateSourceTexture(buffer);

        auto gpuFrame = texture.GetFrame<mediapipe::GpuBuffer>();
        texture.Release();

        auto packet = mediapipe::Adopt(gpuFrame.release());
        return d->graph.AddPacketToInputStream(InputStream, packet.At(timestamp));
    };

    absl::Status added = d->gpuHelper.RunInGlContext(addPacket);
#endif

    if (!added.ok())
        throw Error("Unable to add packet to input stream: " + toString(added));

    if (!d->presencePoller->Next(&d->presencePacket))
        throw Error("Unable to poll for presence of next detection packet.");

    const auto present = d->presencePacket.Get<bool>();

    results->clear();

    if (present)
    {
        if (!d->detectionPoller->Next(&d->detectionPacket))
            throw Error("Unable to poll for next detection packet.");

        const auto& output = d->detectionPacket.Get<std::vector<mediapipe::Detection>>();

        for (const auto& detection : output)
        {
            const auto& box = detection.location_data().relative_bounding_box();

            const auto x = toUint(m_width  * box.xmin());
            const auto y = toUint(m_height * box.ymin());
            const auto w = toUint(m_width  * box.width());
            const auto h = toUint(m_height * box.height());

            results->push_back({ x, y, w, h });
        }
    }
}

// ---------------------------------------------------------------------------------------------- //
