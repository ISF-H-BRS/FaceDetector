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

#include "controlwidget.h"
#include "ui_controlwidget.h"

#include <ifd.h>

#include <QSettings>

// ---------------------------------------------------------------------------------------------- //

namespace {
    static constexpr size_t ScaleCount = 5;
    static constexpr std::array<unsigned int, ScaleCount> Scales = { 1, 2, 4, 8, 16 };
}

// ---------------------------------------------------------------------------------------------- //

ControlWidget::ControlWidget(QWidget* parent)
    : QWidget(parent),
      m_ui(std::make_unique<Ui::ControlWidget>())
{
    m_ui->setupUi(this);

    std::vector<std::string> backends = ifd::FaceDetector::getAvailableBackends();

    for (const auto& backend : backends)
        m_ui->backend->addItem(QString::fromStdString(backend));

    Q_ASSERT(m_ui->backend->count() > 0);
    Q_ASSERT(m_ui->scale->count() == ScaleCount);

    connect(m_ui->backend, SIGNAL(currentTextChanged(QString)),
            this, SIGNAL(backendChanged(QString)));
    connect(m_ui->scale, SIGNAL(currentIndexChanged(int)), this, SLOT(onScaleChanged(int)));
    connect(m_ui->mirror, SIGNAL(toggled(bool)), this, SIGNAL(mirrorChanged(bool)));
}

// ---------------------------------------------------------------------------------------------- //

ControlWidget::~ControlWidget() = default;

// ---------------------------------------------------------------------------------------------- //

auto ControlWidget::backend() const -> QString
{
    return m_ui->backend->currentText();
}

// ---------------------------------------------------------------------------------------------- //

auto ControlWidget::scale() const -> unsigned int
{
    const int index = m_ui->scale->currentIndex();
    Q_ASSERT(0 <= index && index < ScaleCount);

    return Scales.at(index);
}

// ---------------------------------------------------------------------------------------------- //

auto ControlWidget::mirrored() const -> bool
{
    return m_ui->mirror->isChecked();
}

// ---------------------------------------------------------------------------------------------- //

void ControlWidget::saveSettings() const
{
    QSettings settings;
    settings.setValue("currentBackend", m_ui->backend->currentText());
    settings.setValue("currentScale", m_ui->scale->currentText());
}

// ---------------------------------------------------------------------------------------------- //

void ControlWidget::restoreSettings()
{
    QSettings settings;
    m_ui->backend->setCurrentText(settings.value("currentBackend").toString());
    m_ui->scale->setCurrentText(settings.value("currentScale").toString());
}

// ---------------------------------------------------------------------------------------------- //

void ControlWidget::onScaleChanged(int index)
{
    Q_ASSERT(0 <= index && index < ScaleCount);
    emit scaleChanged(Scales.at(index));
}

// ---------------------------------------------------------------------------------------------- //
