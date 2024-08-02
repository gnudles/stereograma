#include <QApplication>
#include "mainwindow.h"
#include <QSurfaceFormat>

#include "parse.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/stereograma.svg"));
    QCoreApplication::setOrganizationName("Kapandaria");
    QCoreApplication::setApplicationName("Stereograma");
    MainWindow w;
    w.show();

    return a.exec();
}
