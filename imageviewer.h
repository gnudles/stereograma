#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QImage>
#include "basicimagewidget.h"

namespace Ui {
    class ImageViewer;
}

class ImageViewer : public QMainWindow, public BasicImageWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = 0);
    ~ImageViewer();
    void setImage(const QImage & image);

private slots:
    void on_actionSave_As_triggered();

    void on_actionInvert_triggered();

private:
    void mouseDoubleClickEvent(QMouseEvent * event);
    Ui::ImageViewer *ui;
};

#endif // IMAGEVIEWER_H
