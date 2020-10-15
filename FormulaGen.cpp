#include "FormulaGen.h"
#include "ui_FormulaGen.h"
#include "exprtk/exprtk.hpp"
#include "stereomaker.h"
#include "mainwindow.h"
#include <QTableWidgetItem>
#include <QSettings>
#include <QListWidget>

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
    QSettings settings;
    formulaList=settings.value("formula",QStringList()<<"0.5+0.5*sin( 100*(((x-0.5)*w/h)^2+(y-0.5)^2)^0.5)").toStringList();
    ui->listWidget->addItems(formulaList);
    ui->formulaPText->setPlainText(formulaList.last());
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
    ui->errorTable->clearContents();
    ui->errorTable->setRowCount(0);
	int w=getOutputImageWidth();
	int h=getOutputImageHeight();
    QString formule_str=ui->formulaPText->toPlainText();
    QByteArray formula_ba=formule_str.toLatin1();
	const char* formula_buf=formula_ba.constData();
	imdata=QImage(w,h,QImage::Format_RGB32);
    
    float x_s, y_s;
    float f_w=w, f_h=h;

    symbol_table_t symbol_table;
    symbol_table.add_pi();
    symbol_table.add_constant("e", M_E);
    symbol_table.add_variable("x", x_s);
    symbol_table.add_variable("y", y_s);
    symbol_table.add_variable("w", f_w);
    symbol_table.add_variable("h", f_h);
    expression_t expression;

    expression.register_symbol_table(symbol_table);
            parser_t::settings_store sstore;
            sstore.disable_all_logic_ops();
            sstore.disable_all_assignment_ops();
            sstore.disable_all_control_structures();
            parser_t parser(sstore);

    bool compile_valid=parser.compile(formula_buf, expression);
    if (compile_valid)
    {
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
        if (!formulaList.contains(formule_str))
        {
            formulaList<<formule_str;
            QSettings settings;
            settings.setValue("formula",formulaList);
            ui->listWidget->addItem(formule_str);
        }


    }
    else {
              ui->errorTable->setRowCount(parser.error_count());
              ui->errorTable->setColumnCount(3);
        for (std::size_t i = 0; i < parser.error_count(); ++i)
              {
                 typedef exprtk::parser_error::type error_t;

                 error_t error = parser.get_error(i);

                 printf("Error[%02d] Position: %02d Type: [%14s] Msg: %s\n",
                        (int)i,
                        (int)error.token.position,
                        exprtk::parser_error::to_str(error.mode).c_str(),
                        error.diagnostic.c_str());
                 ui->errorTable->setItem(i,0,new QTableWidgetItem(QString::number(error.token.position)));
                 ui->errorTable->setItem(i,1,new QTableWidgetItem(tr(exprtk::parser_error::to_str(error.mode).c_str())));
                 ui->errorTable->setItem(i,2,new QTableWidgetItem(tr(error.diagnostic.c_str())));

              }
    }
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

void FormulaGen::on_listWidget_itemActivated(QListWidgetItem *item)
{
    ui->formulaPText->setPlainText(item->text());
}
