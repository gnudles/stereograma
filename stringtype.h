#ifndef STRINGTYPE_H
#define STRINGTYPE_H

#include <QDialog>

namespace Ui {
class StringType;
}

class StringType : public QDialog
{
    Q_OBJECT
    
public:
    explicit StringType(const QString &title, QWidget *parent = 0);
    ~StringType();
    QString getString();
    
private:
    Ui::StringType *ui;
};

#endif // STRINGTYPE_H
