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

#pragma once

#include <QObject>
#include <QImage>

#include <memory>
#include <vector>

class Camera : public QObject
{
    Q_OBJECT

    Camera(const Camera&) = delete;
    Camera(Camera&&) = delete;

    auto operator=(const Camera&) = delete;
    auto operator=(Camera&&) = delete;

public:
    struct CameraInfo
    {
        QString uniqueName;
        QString displayName;

        std::vector<QString> formatStrings;
    };

    using Error = std::runtime_error;

public:
    Camera(size_t cameraIndex, size_t formatIndex, QObject* parent = nullptr);
    ~Camera() override;

    auto formatString() const -> QString;

    auto width() const -> quint32;
    auto height() const -> quint32;

    void start();
    void stop();

    static auto getAvailableCameras() -> std::vector<CameraInfo>;

signals:
    void frameAvailable(const QImage& frame);

private slots:
    void update();

private:
    auto waitForFrame() const -> bool;

private:
    class Private;
    std::unique_ptr<Private> d;
};
