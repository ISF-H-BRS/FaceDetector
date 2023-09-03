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

#include "camera.h"
#include "controlwidget.h"
#include "fpscounter.h"

#include <ifd.h>

#include <QComboBox>
#include <QLabel>
#include <QMainWindow>

#include <memory>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    MainWindow(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;

    auto operator=(const MainWindow&) = delete;
    auto operator=(MainWindow&&) = delete;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void openImage();
    void openCamera();
    void closeImage();

    void updateBackend();

    void showAboutDialog();

    void handleNewFrame(const QImage& frame);
    void handleError(const QString& msg);

private:
    std::unique_ptr<Ui::MainWindow> m_ui;

    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<QImage> m_image;

    std::unique_ptr<ifd::FaceDetector> m_detector;

    std::vector<ifd::Rect> m_results;
    std::vector<QRect> m_rects;

    ControlWidget* m_controlWidget;

    FpsCounter m_fpsCounter;
    QLabel* m_fpsLabel;
};
