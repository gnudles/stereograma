#include "imageviewer.h"
#include "ui_imageviewer.h"
#include <QSettings>
#include <QFileDialog>

ImageViewer::ImageViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    setBasicImageParent(this);
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::setImage(const QImage & image)
{
    imdata= image;
    ui->imageViewArea->setPixmap(QPixmap::fromImage(imdata));
}

void ImageViewer::on_actionSave_As_triggered()
{
    saveAsImage();
}
void ImageViewer::mouseDoubleClickEvent(QMouseEvent * )
{
    on_actionSave_As_triggered();
}

void ImageViewer::on_actionInvert_triggered()
{
    imdata.invertPixels();
    ui->imageViewArea->setPixmap(QPixmap::fromImage(imdata));
}
