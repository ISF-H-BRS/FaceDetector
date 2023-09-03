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

#include "aboutdialog.h"
#include "camerachooserdialog.h"
#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

// ---------------------------------------------------------------------------------------------- //

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      m_ui(std::make_unique<Ui::MainWindow>()),
      m_controlWidget(new ControlWidget),
      m_fpsLabel(new QLabel)
{
    m_ui->setupUi(this);
    m_ui->toolBar->toggleViewAction()->setVisible(false);

    m_controlWidget->setEnabled(false);
    m_ui->toolBar->insertWidget(m_ui->actionAbout, m_controlWidget);

    m_ui->statusbar->addPermanentWidget(m_fpsLabel);

    QSettings settings;
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());

    m_controlWidget->restoreSettings();

    setMinimumWidth(sizeHint().width());

    connect(m_ui->actionOpenImage, SIGNAL(triggered()), this, SLOT(openImage()));
    connect(m_ui->actionOpenCamera, SIGNAL(triggered()), this, SLOT(openCamera()));
    connect(m_ui->actionClose, SIGNAL(triggered()), this, SLOT(closeImage()));
    connect(m_ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    connect(m_controlWidget, SIGNAL(backendChanged(QString)), this, SLOT(updateBackend()));
    connect(m_controlWidget, SIGNAL(scaleChanged(unsigned int)), this, SLOT(updateBackend()));
    connect(m_controlWidget, SIGNAL(mirrorChanged(bool)), m_ui->image, SLOT(setMirrored(bool)));
}

// ---------------------------------------------------------------------------------------------- //

MainWindow::~MainWindow()
{
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("state", saveState());

    m_controlWidget->saveSettings();
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::openImage()
{
    Q_ASSERT(m_image == nullptr && m_camera == nullptr);

    const QString filename =
        QFileDialog::getOpenFileName(this, "Select Image", QString(), "Image Files (*.jpg *.png)");

    if (filename.isEmpty())
        return;

    QImage image(filename);

    if (image.isNull())
    {
        QMessageBox::warning(this, "Error", "Image file doesn't appear to be valid.");
        return;
    }

    m_image = std::make_unique<QImage>(std::move(image));

    m_ui->actionOpenImage->setEnabled(false);
    m_ui->actionOpenCamera->setEnabled(false);
    m_ui->actionClose->setEnabled(true);

    m_controlWidget->setEnabled(true);

    updateBackend();
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::openCamera()
{
    Q_ASSERT(m_image == nullptr && m_camera == nullptr);

    int cameraIndex = -1;
    int formatIndex = -1;

    CameraChooserDialog chooser(&cameraIndex, &formatIndex, this);

    int result = chooser.exec();

    if (result != CameraChooserDialog::Accepted)
        return;

    if (cameraIndex < 0 || formatIndex < 0)
        return;

    try {
        m_camera = std::make_unique<Camera>(cameraIndex, formatIndex);
        connect(m_camera.get(), SIGNAL(frameAvailable(QImage)), this, SLOT(handleNewFrame(QImage)));

        m_fpsCounter.reset();
        m_camera->start();

        m_ui->actionOpenImage->setEnabled(false);
        m_ui->actionOpenCamera->setEnabled(false);
        m_ui->actionClose->setEnabled(true);

        m_controlWidget->setEnabled(true);

        updateBackend();
    }
    catch (const std::exception& e) {
        handleError(e.what());
    }
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::closeImage()
{
    m_detector = nullptr;
    m_image = nullptr;
    m_camera = nullptr;

    m_ui->actionOpenImage->setEnabled(true);
    m_ui->actionOpenCamera->setEnabled(true);
    m_ui->actionClose->setEnabled(false);

    m_controlWidget->setEnabled(false);

    m_ui->image->clear();
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::updateBackend()
{
    Q_ASSERT(m_image != nullptr || m_camera != nullptr);

    try {
        const std::string backend = m_controlWidget->backend().toStdString();
        const unsigned int scale = m_controlWidget->scale();

        const unsigned int width  = (m_image ? m_image->width()  : m_camera->width())  / scale;
        const unsigned int height = (m_image ? m_image->height() : m_camera->height()) / scale;

        m_detector = std::make_unique<ifd::FaceDetector>(width, height, backend);

        m_ui->image->clear();

        if (m_image)
            handleNewFrame(*m_image);

        m_fpsCounter.reset();
    }
    catch (const std::exception& e) {
        handleError(e.what());
    }
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::showAboutDialog()
{
    AboutDialog dialog(this);
    dialog.exec();
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::handleNewFrame(const QImage& frame)
{
    Q_ASSERT(m_detector != nullptr);

    m_ui->image->setImage(frame);

    const auto scale = static_cast<int>(m_controlWidget->scale());

    QImage scaled = frame.scaled(frame.width() / scale, frame.height() / scale,
                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    if (scaled.format() != QImage::Format_RGB32)
        scaled = scaled.convertToFormat(QImage::Format_RGB32);

    const auto data = reinterpret_cast<const ifd::RgbaPixel*>(scaled.bits());
    m_detector->process(std::span(data, scaled.width() * scaled.height()), &m_results);

    m_rects.clear();

    for (const auto& rect : m_results)
    {
        m_rects.emplace_back(scale * rect.x, scale * rect.y,
                             scale * rect.width, scale * rect.height);
    }

    m_ui->image->setRects(m_rects);

    if (m_camera)
    {
        const int fps = m_fpsCounter.update();
        m_fpsLabel->setText(QString("%1 FPS").arg(fps));
    }
}

// ---------------------------------------------------------------------------------------------- //

void MainWindow::handleError(const QString& msg)
{
    closeImage();
    QMessageBox::critical(this, "Error", msg);
}

// ---------------------------------------------------------------------------------------------- //
