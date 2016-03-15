#include "basicimagewidget.h"
#include "imagefiledialog.h"
#include <QSettings>
#include <QString>



BasicImageWidget::BasicImageWidget()
{
    m_parent=0;
}

void BasicImageWidget::saveAsImage()
{
    if (!imdata.isNull())
    {
        QSettings settings;
        QString cdir=settings.value(folderSettings,folderSettings).toString();
        ImageFileDialog dlg(m_parent);

        dlg.init();
        dlg.setFileMode( QFileDialog::AnyFile);
        dlg.setAcceptMode(QFileDialog::AcceptSave);
        dlg.setDirectory(cdir);
        dlg.setNameFilter("Images (*.png *.xpm *.jpg *.jpeg *.bmp *.tiff *.tif *.PNG *.XPM *.JPG *.JPEG *.BMP *.TIF *.TIFF)");
        dlg.setWindowTitle("Save "+saveTitle);
        QString fileName;
        imdata=imdata.convertToFormat(QImage::Format_ARGB32);
        if (dlg.exec())
        {
            fileName = dlg.selectedFiles().first();
            settings.setValue(folderSettings, QFileInfo(fileName).absoluteDir().absolutePath());
            if (!imdata.save(fileName))
            {//probably no file type was supplied...
                fileName.append(".png");
                imdata.save(fileName);
            }
        }
        dlg.deinit();
    }
}

void BasicImageWidget::setFolderSettings(const QString & fsetting)
{
    folderSettings=fsetting;
}
void BasicImageWidget::setFolderSettings(const char * fsetting)
{
    folderSettings=fsetting;
}

void BasicImageWidget::setSaveTitle(const QString& stitle)
{
    saveTitle=stitle;
}

void BasicImageWidget::setSaveTitle(const char * stitle)
{
    saveTitle=stitle;
}

void BasicImageWidget::setBasicImageParent(QWidget *p)
{
    m_parent=p;
}

