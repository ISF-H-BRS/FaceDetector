####################################################################################################
#                                                                                                  #
#   This file is part of the ISF Face Detector project.                                            #
#                                                                                                  #
#   Author:                                                                                        #
#   Marcel Hasler <mahasler@gmail.com>                                                             #
#                                                                                                  #
#   Copyright (c) 2021 - 2023                                                                      #
#   Bonn-Rhein-Sieg University of Applied Sciences                                                 #
#                                                                                                  #
#   This program is free software: you can redistribute it and/or modify it under the terms of     #
#   the GNU General Public License as published by the Free Software Foundation, either            #
#   version 3 of the License, or (at your option) any later version.                               #
#                                                                                                  #
#   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;      #
#   without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.      #
#   See the GNU General Public License for more details.                                           #
#                                                                                                  #
#   You should have received a copy of the GNU General Public License along with this program.     #
#   If not, see <https://www.gnu.org/licenses/>.                                                   #
#                                                                                                  #
####################################################################################################

cmake_minimum_required(VERSION 3.14)
project(FaceViewer LANGUAGES CXX)

set(IFD_USE_QT5 OFF CACHE BOOL "Build against Qt5 rather than Qt6.")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
set(CMAKE_INSTALL_RPATH $ORIGIN)

if (IFD_USE_QT5)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
else()
    find_package(Qt6 COMPONENTS Widgets REQUIRED)
endif()

include_directories(${CMAKE_BINARY_DIR} ../libIFD/include)
link_directories(${CMAKE_BINARY_DIR})

set(PROJECT_SOURCES
    FaceViewer.qrc
    aboutdialog.cpp
    aboutdialog.h
    aboutdialog.ui
    camera.cpp
    camera.h
    camerachooserdialog.cpp
    camerachooserdialog.h
    camerachooserdialog.ui
    controlwidget.cpp
    controlwidget.h
    controlwidget.ui
    fpscounter.cpp
    fpscounter.h
    imagewidget.cpp
    imagewidget.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    rectaverager.cpp
    rectaverager.h
)

add_executable(FaceViewer WIN32 ${PROJECT_SOURCES})
target_link_libraries(FaceViewer PRIVATE Qt::Widgets IFD openpnp-capture)
