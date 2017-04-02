#include "FormulaGen.h"
#include "ui_FormulaGen.h"
#include "parse.h"
#include "stereomaker.h"
#include "mainwindow.h"

FormulaGen::FormulaGen(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::FormulaGen)
{
	ui->setupUi(this);
	setBasicImageParent(this);
}

FormulaGen::~FormulaGen()
{
	delete ui;
}

int FormulaGen::getOutputImageWidth()
{
    return ui->widthSpin->value();
}

int FormulaGen::getOutputImageHeight()
{
    return ui->heightSpin->value();
}
void FormulaGen::on_generatePb_clicked()
{
	int w=getOutputImageWidth();
	int h=getOutputImageHeight();
	QByteArray formula_ba=ui->formulaPText->toPlainText().toAscii();
	const char* formula_buf=formula_ba.constData();
	imdata=QImage(w,h,QImage::Format_RGB32);
    
	bool ok;
	int val;
    for (int x=0;x<w;++x)
	{
		for (int y=0;y<h;++y)
		{
			val=parseXY(formula_buf,(float)x/w,(float)y/h,&ok)*255.99;
			if (val<0)
				val=0;
			else if (val>255)
				val=255;
			imdata.setPixel(x,y,0xff000000|(val<<16)|(val<<8)|val);
		}
	}
	//imdata=imdata.convertToFormat(QImage::Format_Indexed8,StereoMaker::getGrayScale());
	ui->imageViewArea->setPixmap(QPixmap::fromImage(imdata));
}

void FormulaGen::on_actionSave_Image_triggered()
{
    saveAsImage();
}

void FormulaGen::on_actionPush_Image_triggered()
{
    ((MainWindow*)parentWidget())->setDepthImage(imdata);
}

void FormulaGen::on_actionPush_Image_as_Compose_Pattern_triggered()
{
    ((MainWindow*)parentWidget())->setComposePattern(imdata);
}
