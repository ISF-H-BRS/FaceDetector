cc_binary(
    name  = "mediapipe",
    srcs  = ["mediapipebackendimpl.cpp", "mediapipebackendimpl.h", "ifd.h"],
    copts = ["-std=c++20"],

    deps  = [
        "//mediapipe/calculators/core:packet_presence_calculator",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/port:parse_text_proto",
    ] + select({
        "//mediapipe/gpu:disable_gpu": [
            "//mediapipe/modules/face_detection:face_detection_full_range_cpu",
        ],
        "//conditions:default": [
            "//mediapipe/gpu:gl_calculator_helper",
            "//mediapipe/gpu:gpu_buffer",
            "//mediapipe/gpu:gpu_shared_data_internal",
            "//mediapipe/modules/face_detection:face_detection_full_range_gpu",
        ],
    }),

    linkshared = 1,
)
