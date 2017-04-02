#ifndef FORMULAGEN_H
#define FORMULAGEN_H

#include <QMainWindow>
#include "basicimagewidget.h"
namespace Ui {
class FormulaGen;
}

class FormulaGen : public QMainWindow, public BasicImageWidget
{
	Q_OBJECT
	
public:
	explicit FormulaGen(QWidget *parent = 0);
	~FormulaGen();
	int getOutputImageWidth();
	int getOutputImageHeight();
	
private slots:
	void on_generatePb_clicked();
	
	void on_actionSave_Image_triggered();
	
	void on_actionPush_Image_triggered();
	
	void on_actionPush_Image_as_Compose_Pattern_triggered();
	
private:
	Ui::FormulaGen *ui;
};

#endif // FORMULAGEN_H
