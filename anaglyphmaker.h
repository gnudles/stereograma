#ifndef ANAGLYPHMAKER_H
#define ANAGLYPHMAKER_H

#include <QMainWindow>

namespace Ui {
class AnaglyphMaker;
}

class AnaglyphMaker : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit AnaglyphMaker(QWidget *parent = 0);
    ~AnaglyphMaker();
    
private slots:
    void on_mergeButton_clicked();

private:
    Ui::AnaglyphMaker *ui;
};

#endif // ANAGLYPHMAKER_H
