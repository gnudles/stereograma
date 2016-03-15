#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QLabel>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "imageviewer.h"
#include "basicimagewidget.h"

class ImageContainerWidget : public QLabel, public BasicImageWidget
{
    Q_OBJECT
public:
    ImageContainerWidget(QWidget *parent = 0);
    void openDialog();
    void setImage(const QImage &image);
private slots:
    void downloadFinished(QNetworkReply* reply);
    void showContextMenu(const QPoint&);
private:

    void setImage(const QString &fileName);

    const QString & getText();
    void mySetPixmap();
    void saveDialog();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void mouseDoubleClickEvent(QMouseEvent * event);

    QAction *_saveAsAction;
    QAction *_unloadAction;
    QAction *_loadAction;
    QAction *_viewAction;
    ImageViewer *m_viewer;
    QMenu myMenu;
    QString replaceText;
    QNetworkAccessManager netManager;
};



#endif // IMAGEWIDGET_H
