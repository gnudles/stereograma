#ifndef BASICIMAGEWIDGET_H
#define BASICIMAGEWIDGET_H
#include <QImage>
#include <QWidget>

class BasicImageWidget
{
public:
    BasicImageWidget();
    void saveAsImage();
    void setFolderSettings(const QString &fsetting);
    void setFolderSettings(const char *fsetting);
    void setSaveTitle(const QString &stitle);
    void setSaveTitle(const char *stitle);
    void setBasicImageParent(QWidget *p);
    const QImage & getImage() {return imdata;}
    const QImage * getImagePtr() {return &imdata;}
protected:
    QImage imdata;
    QString folderSettings;
    QString saveTitle;
    QWidget *m_parent;
};

#endif // BASICIMAGEWIDGET_H
