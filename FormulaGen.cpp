#include "FormulaGen.h"
#include "ui_FormulaGen.h"
#include "exprtk/exprtk.hpp"
#include "stereomaker.h"
#include "mainwindow.h"

typedef exprtk::symbol_table<float> symbol_table_t;
typedef exprtk::expression<float> expression_t;
typedef exprtk::parser<float> parser_t;

FormulaGen::FormulaGen(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::FormulaGen)
{
	ui->setupUi(this);
	setBasicImageParent(this);
	setSaveTitle("Formula Depth Image");
	setFolderSettings("depthmaps");
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
	QByteArray formula_ba=ui->formulaPText->toPlainText().toLatin1();
	const char* formula_buf=formula_ba.constData();
	imdata=QImage(w,h,QImage::Format_RGB32);
    
    float x_s, y_s;
    symbol_table_t symbol_table;
    symbol_table.add_pi();
    symbol_table.add_constant("e", M_E);
    symbol_table.add_variable("x", x_s);
    symbol_table.add_variable("y", y_s);
    expression_t expression;

    expression.register_symbol_table(symbol_table);
            parser_t::settings_store sstore;
            sstore.disable_all_logic_ops();
            sstore.disable_all_assignment_ops();
            sstore.disable_all_control_structures();
            parser_t parser(sstore);

    bool compile_valid=parser.compile(formula_buf, expression);
	bool ok;
	int val;
    for (int x=0;x<w;++x)
	{
		for (int y=0;y<h;++y)
		{
            x_s = (float)x/w;
            y_s = (float)y/h;
            val=expression.value()*255.99;

            if (std::isnan(val) || std::isinf(val) || val<0)
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
