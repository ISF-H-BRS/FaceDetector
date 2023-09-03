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

#include <QDialog>

#include <memory>

namespace Ui {
    class CameraChooserDialog;
}

class CameraChooserDialog : public QDialog
{
    Q_OBJECT

    CameraChooserDialog(const CameraChooserDialog&) = delete;
    CameraChooserDialog(CameraChooserDialog&&) = delete;

    auto operator=(const CameraChooserDialog&) = delete;
    auto operator=(CameraChooserDialog&&) = delete;

public:
    CameraChooserDialog(int* cameraIndex, int* formatIndex, QWidget* parent = nullptr);
    ~CameraChooserDialog() override;

private slots:
    void updateFormats(int index);
    void saveData();

private:
    std::unique_ptr<Ui::CameraChooserDialog> m_ui;

    int* m_cameraIndex;
    int* m_formatIndex;

    std::vector<Camera::CameraInfo> m_cameras;
};
