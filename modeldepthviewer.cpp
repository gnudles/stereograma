#include "modeldepthviewer.h"
#include "ui_modeldepthviewer.h"
#include <QSettings>
#include <QFileDialog>
#include "stereomaker.h"
#include "mainwindow.h"

ModelDepthViewer::ModelDepthViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ModelDepthViewer)
{
    ui->setupUi(this);
    ui->modelview->setSaveTitle("Rendered Depth Image");
    ui->modelview->setFolderSettings("depthmaps");
    ui->modelview->setBasicImageParent(this);
}

int ModelDepthViewer::getOutputImageWidth()
{
    return ui->widthSpin->value();
}

int ModelDepthViewer::getOutputImageHeight()
{
    return ui->heightSpin->value();
}
void ModelDepthViewer::setModel(Model3D *model)
{
    ui->modelview->setModel(model);
    ui->modelview->setParams(ui->moveXSlider->value()/8.0,ui->moveYSlider->value()/8.0,
                             ui->zoomSlider->value(),ui->contrastSlider->value(),
                             ui->hSliderRotX->value(),ui->hSliderRotY->value(),ui->hSliderRotZ->value(),ui->scaleSlider->value());
    ui->modelview->repaint();
}

ModelDepthViewer::~ModelDepthViewer()
{
    delete ui;
}


void ModelDepthViewer::on_hSliderRotZ_valueChanged(int value)
{
    ui->modelview->setRotationZ(value);
    ui->modelview->repaint();
}

void ModelDepthViewer::on_hSliderRotX_valueChanged(int value)
{
    ui->modelview->setRotationX(value);
    ui->modelview->repaint();
}

void ModelDepthViewer::on_hSliderRotY_valueChanged(int value)
{
    ui->modelview->setRotationY(value);
    ui->modelview->repaint();
}

void ModelDepthViewer::on_actionSave_Image_triggered()
{
    ui->modelview->snapShot();
    ui->modelview->saveAsImage();
}

void ModelDepthViewer::on_zoomSlider_valueChanged(int value)
{
    ui->modelview->setZoom(value);
    ui->modelview->repaint();
}
void ModelDepthViewer::on_scaleSlider_valueChanged(int value)
{
    ui->modelview->setScale(value);
    ui->modelview->repaint();
}

void ModelDepthViewer::on_contrastSlider_valueChanged(int value)
{
    ui->modelview->setContrast(value);
    ui->modelview->repaint();
}

void ModelDepthViewer::on_actionPush_Image_triggered()
{
    const QImage * im=ui->modelview->snapShot();
    if (im!=0)
        ((MainWindow*)parentWidget())->setDepthImage(*im);

}

void ModelDepthViewer::on_moveXSlider_valueChanged(int value)
{
    ui->modelview->setXOffset(value/8.0);
    ui->modelview->repaint();
}

void ModelDepthViewer::on_moveYSlider_valueChanged(int value)
{
    ui->modelview->setYOffset(value/8.0);
    ui->modelview->repaint();
}

void ModelDepthViewer::on_widthSpin_editingFinished()
{
    ui->modelview->setViewport(getOutputImageWidth(),getOutputImageHeight());
    ui->modelview->repaint();
}

void ModelDepthViewer::on_heightSpin_editingFinished()
{
    ui->modelview->setViewport(getOutputImageWidth(),getOutputImageHeight());
    ui->modelview->repaint();
}

