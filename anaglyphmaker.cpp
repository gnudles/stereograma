#include "anaglyphmaker.h"
#include "imageviewer.h"
#include "ui_anaglyphmaker.h"
#include <QMessageBox>

AnaglyphMaker::AnaglyphMaker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AnaglyphMaker)
{
    ui->setupUi(this);
    ui->rightView->setBasicImageParent(this);
    ui->rightView->setFolderSettings("anaglyph");
    ui->rightView->setSaveTitle("Right Image");
    ui->leftView->setBasicImageParent(this);
    ui->leftView->setFolderSettings("anaglyph");
    ui->leftView->setSaveTitle("Left Image");
}

AnaglyphMaker::~AnaglyphMaker()
{
    delete ui;
}

void AnaglyphMaker::on_mergeButton_clicked()
{
    if (ui->leftView->getImage().isNull() || ui->rightView->getImage().isNull())
    {
        QMessageBox::warning(this,"Error","Please load images");
        return;
    }
    if ((ui->leftView->getImage().width() != ui->rightView->getImage().width())
        || (ui->leftView->getImage().height() != ui->rightView->getImage().height()))
    {
        QMessageBox::warning(this,"Error","Images are not at the same size.");
        return;
    }
    int width = ui->leftView->getImage().width();
    int height = ui->leftView->getImage().height();

    const QImage * refLeft=ui->leftView->getImagePtr();
    const QImage * refRight=ui->rightView->getImagePtr();
    if (ui->swapButton->isChecked())
    {
        const QImage *temp=refLeft;
        refLeft=refRight;
        refRight=temp;
    }
    QImage imLeft,imRight;
    if (ui->leftView->getImage().depth()!=32)
        imLeft=refLeft->convertToFormat(QImage::Format_ARGB32);
    else
        imLeft=*refLeft;

    if (ui->rightView->getImage().depth()!=32)
        imRight=refRight->convertToFormat(QImage::Format_ARGB32);
    else
        imRight=*refRight;

    QImage result(width,height,QImage::Format_ARGB32);
    unsigned int *resptr;
    unsigned int *endptr;
    const unsigned int *leftptr;
    const unsigned int *rightptr;
    unsigned int rightalpha,leftalpha;
    unsigned int alpha;
    unsigned int r,g,b;
    for (int i=0;i<height;i++)
    {
        resptr=(unsigned int *)result.scanLine(i);
        endptr=resptr+width;
        leftptr=(const unsigned int *)imLeft.scanLine(i);
        rightptr=(const unsigned int *)imRight.scanLine(i);
        while(resptr<endptr)
        {
            rightalpha=((*rightptr)&0xff000000)>>24;
            leftalpha=((*leftptr)&0xff000000)>>24;
            r=((*leftptr)&0x00ff0000)>>16;
            g=((*rightptr)&0x0000ff00)>>8;
            b=((*rightptr)&0x000000ff);
            alpha = leftalpha+rightalpha-rightalpha*leftalpha/255;
            if (alpha==0)
            {
                *resptr=0;
                resptr++;
                rightptr++;
                leftptr++;
            }
            else
            {
                r=(unsigned char)((leftalpha*r)/(alpha));

                g=(unsigned char)((rightalpha*g)/(alpha));

                b=(unsigned char)((rightalpha*b)/(alpha));
                *resptr=(alpha<<24)|(r<<16)|(g<<8)|b;
                resptr++;
                rightptr++;
                leftptr++;
            }
        }
    }
    ImageViewer * imview=new ImageViewer(this);
    imview->setWindowTitle("Result");
    imview->setFolderSettings("anaglyph_result");
    imview->setSaveTitle("Anaglyph");
    imview->setImage(result);
    imview->show();
}
