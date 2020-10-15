#include <QApplication>
#include "mainwindow.h"
#include <QGLFormat>

#include "parse.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(4);
    QGLFormat::setDefaultFormat(glf);
    a.setWindowIcon(QIcon(":/images/stereograma.svg"));
    QCoreApplication::setOrganizationName("Kapandaria");
    QCoreApplication::setApplicationName("Stereograma");
    MainWindow w;
    w.show();

    return a.exec();
}
