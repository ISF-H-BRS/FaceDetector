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

#include "camerachooserdialog.h"
#include "ui_camerachooserdialog.h"

#include <QPushButton>
#include <QSettings>

// ---------------------------------------------------------------------------------------------- //

CameraChooserDialog::CameraChooserDialog(int* cameraIndex, int* formatIndex, QWidget* parent)
    : QDialog(parent),
      m_ui(std::make_unique<Ui::CameraChooserDialog>()),
      m_cameraIndex(cameraIndex),
      m_formatIndex(formatIndex)
{
    Q_ASSERT(cameraIndex != nullptr && formatIndex != nullptr);

    m_ui->setupUi(this);

    m_cameras = Camera::getAvailableCameras();

    if (m_cameras.empty())
        m_ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
    {
        for (const auto& camera : m_cameras)
            m_ui->camera->addItem(camera.displayName);

        QSettings settings;
        m_ui->camera->setCurrentText(settings.value("currentCamera").toString());

        updateFormats(m_ui->camera->currentIndex());

        m_ui->format->setCurrentText(settings.value("currentFormat").toString());
    }

    connect(m_ui->camera, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFormats(int)));
    connect(this, SIGNAL(accepted()), this, SLOT(saveData()));
}

// ---------------------------------------------------------------------------------------------- //

CameraChooserDialog::~CameraChooserDialog()
{
    QSettings settings;
    settings.setValue("currentCamera", m_ui->camera->currentText());
    settings.setValue("currentFormat", m_ui->format->currentText());
}

// ---------------------------------------------------------------------------------------------- //

void CameraChooserDialog::updateFormats(int index)
{
    Q_ASSERT(index >= 0);

    m_ui->format->clear();

    for (const QString& format : m_cameras.at(index).formatStrings)
        m_ui->format->addItem(format);
}

// ---------------------------------------------------------------------------------------------- //

void CameraChooserDialog::saveData()
{
    Q_ASSERT(m_cameraIndex != nullptr && m_formatIndex != nullptr);

    *m_cameraIndex = m_ui->camera->currentIndex();
    *m_formatIndex = m_ui->format->currentIndex();
}

// ---------------------------------------------------------------------------------------------- //
