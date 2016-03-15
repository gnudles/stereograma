#include "imagecontainerwidget.h"
#include <QDragEnterEvent>
#include <QUrl>
#include <QSettings>
#include "imagefiledialog.h"
#include <QMessageBox>
#include <QMimeData>



ImageContainerWidget::ImageContainerWidget(QWidget *parent) :
    QLabel(parent)
{

    connect(&netManager, SIGNAL(finished(QNetworkReply*)),
                SLOT(downloadFinished(QNetworkReply*)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
                SLOT(showContextMenu(const QPoint&)));
    _loadAction=myMenu.addAction("Load");
    _saveAsAction=myMenu.addAction("Save As...");
    _viewAction=myMenu.addAction("View");
    _unloadAction=myMenu.addAction("Unload");
    m_viewer=new ImageViewer(this);
    setAcceptDrops(true);
}
const QString & ImageContainerWidget::getText()
{
    if (replaceText.isNull())
    {
        replaceText=text();
    }
    return replaceText;
}


void ImageContainerWidget::mySetPixmap()
{
    if (replaceText.isNull())
    {
        replaceText=text();
    }
    if (imdata.isNull())
    {
        setAlignment(Qt::AlignLeft | Qt::AlignTop);
        setText(replaceText);
    }
    else
    {
        setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        setScaledContents(false);
        QSize size = QSize(width(),height());
        QImage smallimage=imdata.convertToFormat(QImage::Format_ARGB32).scaled(size, Qt::KeepAspectRatio);
        setPixmap(QPixmap::fromImage(smallimage));

    }
}
void ImageContainerWidget::setImage(const QString &fileName)
{
    if(!imdata.load(fileName))
        QMessageBox::warning(this,"Error","Unrecognized image format");
    mySetPixmap();
}

void ImageContainerWidget::setImage(const QImage &image)
{
    imdata=image;
    mySetPixmap();
}

void ImageContainerWidget::downloadFinished(QNetworkReply* reply)
{
    if (!reply->error())
    {
        if(!imdata.load((QIODevice*)reply,0))
            QMessageBox::warning(this,"Error","Unrecognized image format");
        mySetPixmap();
    }
    else
    {
        QMessageBox::warning(this,"Error","Could not download the file");
    }
    reply->deleteLater();
}
void ImageContainerWidget::showContextMenu(const QPoint& pos)
{
    QPoint globalPos = mapToGlobal(pos);
    QAction* selectedItem = myMenu.exec(globalPos);
    if (selectedItem == _saveAsAction)
    {
        saveDialog();
    }
    else if (selectedItem == _unloadAction)
    {
        imdata= QImage();
        mySetPixmap();
    }
    else if(selectedItem== _loadAction)
    {
        openDialog();
    }
    else if(selectedItem== _viewAction)
    {
        m_viewer->setFolderSettings(folderSettings);
        m_viewer->setSaveTitle(saveTitle);
        m_viewer->setImage(imdata);
        m_viewer->show();
    }
}



void ImageContainerWidget::saveDialog()
{
    saveAsImage();
}

void ImageContainerWidget::openDialog()
{
    QSettings settings;
    QString cdir=settings.value(folderSettings,folderSettings).toString();

    ImageFileDialog dlg(this);

    dlg.init();
    dlg.setFileMode( QFileDialog::ExistingFile);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setDirectory(cdir);
    dlg.setNameFilter(tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp *.gif *.tiff *.tif *.PNG *.XPM *.JPG *.JPEG *.BMP *.GIF *.TIF *.TIFF)"));
    dlg.setWindowTitle("Open "+getText());
    QString fileName;

    if (dlg.exec())
    {
        fileName = dlg.selectedFiles().first();
        settings.setValue(folderSettings, QFileInfo(fileName).absoluteDir().absolutePath());
        setImage(fileName);
    }
    dlg.deinit();

}

void ImageContainerWidget::mouseDoubleClickEvent(QMouseEvent * )
{
    openDialog();
}
void ImageContainerWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}
void ImageContainerWidget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty())
        return;
    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty())
    {
        QNetworkRequest request(urls.first());
        netManager.get(request);
    }
    else
    {
        setImage(fileName);
    }
}
