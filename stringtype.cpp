#include "stringtype.h"
#include "ui_stringtype.h"

StringType::StringType(const QString & title,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StringType)
{
    ui->setupUi(this);
    setWindowTitle(title);
}

StringType::~StringType()
{
    delete ui;
}

QString StringType::getString()
{
    return ui->lineEdit->text();
}
