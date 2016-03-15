#ifndef PRESETEDIT_H
#define PRESETEDIT_H

#include <QWidget>
#include "preset.h"
namespace Ui {
class PresetEdit;
}

class PresetEdit : public QWidget
{
    Q_OBJECT
    
public:
    explicit PresetEdit(QWidget *parent = 0);
    ~PresetEdit();
    void setPreset(Preset* preset);
signals:
    void onEdited();
private slots:
    void on_crossEyedCheckBox_stateChanged(int state);

    void on_dpiSpinBox_valueChanged(int dpi);

    void on_minimumDepthDoubleSpinBox_valueChanged(double mindepth);

    void on_maximumDepthDoubleSpinBox_valueChanged(double maxdepth);

    void on_eyeSeperationDoubleSpinBox_valueChanged(double eyesep);

    void on_observerDistanceInDoubleSpinBox_valueChanged(double obsdist);

    void on_imageHeightSpinBox_valueChanged(int height);

    void on_imageWidthSpinBox_valueChanged(int width);

private:
    Ui::PresetEdit *ui;
    Preset *m_preset;
};

#endif // PRESETEDIT_H
