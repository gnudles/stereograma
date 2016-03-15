#include <QApplication>
#include "mainwindow.h"
#include <QGLFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(4);
    QGLFormat::setDefaultFormat(glf);
    a.setWindowIcon(QIcon(":/images/stereograma.svg"));
    MainWindow w;
    w.show();

    return a.exec();
}
