####################################################################################################
#                                                                                                  #
#   This file is part of the ISF Face Detector library.                                            #
#                                                                                                  #
#   Author:                                                                                        #
#   Marcel Hasler <mahasler@gmail.com>                                                             #
#                                                                                                  #
#   Copyright (c) 2021 - 2023                                                                      #
#   Bonn-Rhein-Sieg University of Applied Sciences                                                 #
#                                                                                                  #
#   This library is free software: you can redistribute it and/or modify it under the terms of     #
#   the GNU Lesser General Public License as published by the Free Software Foundation, either     #
#   version 3 of the License, or (at your option) any later version.                               #
#                                                                                                  #
#   This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;      #
#   without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.      #
#   See the GNU Lesser General Public License for more details.                                    #
#                                                                                                  #
#   You should have received a copy of the GNU Lesser General Public License along with this       #
#   library. If not, see <https://www.gnu.org/licenses/>.                                          #
#                                                                                                  #
####################################################################################################

cmake_minimum_required(VERSION 3.14)
project(IFD LANGUAGES CXX)

set(IFD_USE_LIBFACEDETECTION ON CACHE BOOL "Build libFaceDetection backend.")
set(IFD_USE_DLIB ON CACHE BOOL "Build Dlib backend.")
set(IFD_USE_MEDIAPIPE ON CACHE BOOL "Build MediaPipe backend.")
set(IFD_USE_OPENCV ON CACHE BOOL "Build OpenCV backend.")

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH $ORIGIN)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mavx2 -ffast-math -fvisibility=hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-enum-enum-conversion")

include_directories(${CMAKE_BINARY_DIR} include)
link_directories(${CMAKE_BINARY_DIR})

set(IFD_SOURCES
    include/ifd.h
    backend.h
    convert.cpp
    convert.h
    dummybackend.cpp
    dummybackend.h
    ifd.cpp
    namespace.h
)

if (WIN32)
    set(IFD_LIBRARIES tbb12)
else()
    set(IFD_LIBRARIES tbb)
endif()

if (IFD_USE_LIBFACEDETECTION)
    add_compile_definitions(IFD_USE_LIBFACEDETECTION)
    set(IFD_SOURCES ${IFD_SOURCES} libfacedetectionbackend.cpp libfacedetectionbackend.h)
    set(IFD_LIBRARIES ${IFD_LIBRARIES} facedetection)
endif()

if (IFD_USE_DLIB)
    add_compile_definitions(IFD_USE_DLIB)
    set(IFD_SOURCES ${IFD_SOURCES} dlibbackend.cpp dlibbackend.h)
    set(IFD_LIBRARIES ${IFD_LIBRARIES} dlib lapack)
endif()

if (IFD_USE_MEDIAPIPE)
    add_compile_definitions(IFD_USE_MEDIAPIPE)
    set(IFD_SOURCES ${IFD_SOURCES} mediapipebackend.cpp mediapipebackend.h)
    set(IFD_LIBRARIES ${IFD_LIBRARIES} mediapipe)

    add_custom_target(MediaPipeBackendImpl SOURCES
        mediapipe/mediapipebackendimpl.cpp
        mediapipe/mediapipebackendimpl.h
        mediapipe/BUILD
        mediapipe/README
    )

    # Allow editors to find headers
    if (DEFINED ENV{MEDIAPIPE_HOME})
        set(MEDIAPIPE_HOME $ENV{MEDIAPIPE_HOME})
        set(MEDIAPIPE_BASE ${MEDIAPIPE_HOME}/bazel-mediapipe)
        set(MEDIAPIPE_BIN  ${MEDIAPIPE_BASE}/bazel-out/host/bin)

        include_directories(
            ${MEDIAPIPE_HOME}
            ${MEDIAPIPE_BASE}/external/com_google_absl
            ${MEDIAPIPE_BIN}
            ${MEDIAPIPE_BIN}/external/com_github_gflags_gflags/_virtual_includes/gflags
            ${MEDIAPIPE_BIN}/external/com_github_glog_glog/_virtual_includes/default_glog_headers
        )
    endif()
endif()

if (IFD_USE_OPENCV)
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})

    add_custom_command(
        OUTPUT opencvmodel.h
        COMMAND python3 makeheader.py haarcascade_frontalface_default.xml
                                      ${CMAKE_BINARY_DIR}/opencvmodel.h opencvmodel
        DEPENDS "${CMAKE_SOURCE_DIR}/opencv/haarcascade_frontalface_default.xml"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/opencv"
        COMMENT "Generating header file for OpenCV model ..."
    )

    add_compile_definitions(IFD_USE_OPENCV)
    set(IFD_SOURCES ${IFD_SOURCES} opencvbackend.cpp opencvbackend.h opencvmodel.h)
    set(IFD_LIBRARIES ${IFD_LIBRARIES} opencv_objdetect)
endif()

add_library(IFD SHARED ${IFD_SOURCES})
target_link_libraries(IFD ${IFD_LIBRARIES})

if (WIN32)
    target_compile_definitions(IFD PRIVATE IFD_BUILD_PROCESS)
    set_target_properties(IFD PROPERTIES PREFIX "")
endif()
