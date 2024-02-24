#include <QApplication>
#include "mainwindow.h"
#include <QSurfaceFormat>

#include "parse.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSurfaceFormat glf = QSurfaceFormat::defaultFormat();
    //TODO: glf.setSampleBuffers(true);
    glf.setSamples(4);
    QSurfaceFormat::setDefaultFormat(glf);
    a.setWindowIcon(QIcon(":/images/stereograma.svg"));
    QCoreApplication::setOrganizationName("Kapandaria");
    QCoreApplication::setApplicationName("Stereograma");
    MainWindow w;
    w.show();

    return a.exec();
}
