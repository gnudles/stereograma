#ifndef MODELDEPTHVIEWER_H
#define MODELDEPTHVIEWER_H

#include <QMainWindow>
#include "model3d.h"

namespace Ui {
class ModelDepthViewer;
}

class ModelDepthViewer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ModelDepthViewer(QWidget *parent = 0);
    int getOutputImageWidth();
    int getOutputImageHeight();
    ~ModelDepthViewer();
    void setModel(Model3D * model);

private slots:

    void on_hSliderRotZ_valueChanged(int value);

    void on_hSliderRotY_valueChanged(int value);

    void on_hSliderRotX_valueChanged(int value);

    void on_actionSave_Image_triggered();

    void on_zoomSlider_valueChanged(int value);

    void on_scaleSlider_valueChanged(int value);

    void on_contrastSlider_valueChanged(int value);

    void on_actionPush_Image_triggered();

    void on_moveXSlider_valueChanged(int value);

    void on_moveYSlider_valueChanged(int value);

    void on_widthSpin_editingFinished();

    void on_heightSpin_editingFinished();

private:
    Ui::ModelDepthViewer *ui;
};

#endif // MODELDEPTHVIEWER_H
