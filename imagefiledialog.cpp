#include "imagefiledialog.h"
#include <QMessageBox>
#include <QLayout>
#include <QSplitter>
#include <QDebug>


void ImageFileDialog::init()
{
        _preview = 0;
        QSplitter *splitter;
        splitter = ((QFileDialog*)this)->findChild<QSplitter*>("splitter");
        if (splitter == 0)
        {
            return;
        }

        _preview = new QLabel();
        QRect geomerty = _preview->geometry();
        geomerty.setWidth(160);

        _preview->setGeometry(geomerty);
        splitter->addWidget(_preview);
        connect(this, SIGNAL(currentChanged(QString)),
                this, SLOT(fileChanged(QString)));

}
void ImageFileDialog::deinit()
{
        if (_preview != 0)
        {
            delete _preview;
            _preview=0;
        }
}
void ImageFileDialog::fileChanged(const QString &file)
{
    if (_preview == 0)
    {
        return;
    }
    QImage img=QImage(file);
    if (img.isNull())
    {
        _preview->setPixmap(QPixmap());
    }
    else
    {
        QSize size = _preview->size();
        QImage smallimage=img.convertToFormat(QImage::Format_ARGB32).scaled(size, Qt::KeepAspectRatio);
        QPixmap pix=QPixmap::fromImage(smallimage);
        _preview->setPixmap(pix);
    }
}
