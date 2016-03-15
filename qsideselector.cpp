#include "qsideselector.h"
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>

QSideSelector::QSideSelector(QWidget *parent) :
    QWidget(parent)
{
}

void QSideSelector::paintEvent(QPaintEvent * event)
{
    QPainter *qp=new QPainter((QPaintDevice *)this);
    QBrush brush(Qt::SolidPattern);
    qp->setPen(Qt::black);
    brush.setColor(QColor(60,55,50));
    qp->setBrush(brush);
    qp->setRenderHint(QPainter::HighQualityAntialiasing,true);
    qp->setRenderHint(QPainter::Antialiasing,true);
    qp->drawRoundedRect(1,1,width()-2,height()-2,10,10);
    brush.setColor(QColor(255,0,0));
    qp->setBrush(brush);
    qp->drawEllipse(2,2,16,16);
    qp->end();
    delete qp;
}

void QSideSelector::mouseMoveEvent (QMouseEvent * event)
{
    if (event->buttons()&m_mouse_capture_buttons!=0)
        qDebug("hello %d",event->x());
}

void QSideSelector::mousePressEvent (QMouseEvent * event)
{

   // m_mouse_capture_buttons=event->buttons();

}

void QSideSelector::mouseReleaseEvent (QMouseEvent * event)
{
    m_mouse_capture_buttons=0;
}
