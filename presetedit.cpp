#include "presetedit.h"
#include "ui_presetedit.h"

PresetEdit::PresetEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PresetEdit)
{
    ui->setupUi(this);
    m_preset=0;
}

PresetEdit::~PresetEdit()
{
    delete ui;
}

void PresetEdit::setPreset(Preset *preset)
{
    m_preset=preset;
    ui->crossEyedCheckBox->setChecked(!preset->getIsParallel());
    ui->dpiSpinBox->setValue(preset->getDotsPerInch());
    ui->maximumDepthDoubleSpinBox->setValue(preset->getMaximumDepth());
    ui->minimumDepthDoubleSpinBox->setValue(preset->getMinimumDepth());
    ui->eyeSeperationDoubleSpinBox->setValue(preset->getEyeSeperation());
    ui->imageHeightSpinBox->setValue(preset->getResultHeight());
    ui->imageWidthSpinBox->setValue(preset->getResultWidth());
    ui->observerDistanceInDoubleSpinBox->setValue(preset->getObserverDistance());
}

void PresetEdit::on_crossEyedCheckBox_stateChanged(int state)
{
    m_preset->setIsParallel(!state);
    emit(onEdited());
}

void PresetEdit::on_dpiSpinBox_valueChanged(int dpi)
{
    m_preset->setDotsPerInch(dpi);
    emit(onEdited());
}

void PresetEdit::on_minimumDepthDoubleSpinBox_valueChanged(double mindepth)
{
    m_preset->setMinimumDepth(mindepth);
    emit(onEdited());
}

void PresetEdit::on_maximumDepthDoubleSpinBox_valueChanged(double maxdepth)
{
    ui->minimumDepthDoubleSpinBox->setMaximum(maxdepth-1);
    m_preset->setMaximumDepth(maxdepth);
    emit(onEdited());
}

void PresetEdit::on_eyeSeperationDoubleSpinBox_valueChanged(double eyesep)
{
    m_preset->setEyeSeperation(eyesep);
    emit(onEdited());
}

void PresetEdit::on_observerDistanceInDoubleSpinBox_valueChanged(double obsdist)
{
    m_preset->setObserverDistance(obsdist);
    emit(onEdited());
}

void PresetEdit::on_imageHeightSpinBox_valueChanged(int height)
{
    m_preset->setResultHeight(height);
    emit(onEdited());
}

void PresetEdit::on_imageWidthSpinBox_valueChanged(int width)
{
    m_preset->setResultWidth(width);
    emit(onEdited());
}
