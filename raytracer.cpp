#include "raytracer.h"
#include <QVector3D>
#include <QVector2D>
#include <math.h>

RayTracer::RayTracer(float x, float y, float z)
{

}


QImage RayTracer::Render()
{
    QImage result=QImage(200,200,QImage::Format_Indexed8);
    int colorsetter;
    for (colorsetter = 0 ; colorsetter < 256 ; colorsetter++)
        result.setColor(colorsetter,qRgb(colorsetter,colorsetter,colorsetter));
    result.fill(0);
    /*
    result.setPixel(0,0,255);
    QVector3D p1=QVector3D(0.5,0.2,0.4);
    QVector3D p2=QVector3D(0,-0.3,0.5);
    QVector3D p3=QVector3D(-0.4,0.2,0.1);
    QVector2D topleftcorner=QVector2D(-0.4,0.2);
    QVector2D bottomrightcorner=QVector2D(0.5,-0.3);
    */
    //int mins=(result.width()>result.height())?result.height():result.width();
    QVector2D p1(10,20);
    QVector2D p2(5,30);
    QVector2D p3(50,50);
    int i,j;
    float position;
    int jstart,jend;
    int ydiff=p2.y()-p1.y();
    if (ydiff==0)
        ydiff=1;
    int ydiff2=p3.y()-p1.y();
    if (ydiff2==0)
        ydiff2=1;

    for(i=0;i<= (p2.y()-p1.y()); i++)
    {
        position=(float)i/ydiff;

        jstart=p1.x()+position*(p2.x()-p1.x());

        jend= ceil(p1.x()+((float)i/ydiff2)*(p3.x()-p1.x()));
        if (jstart>jend)
        {
            jstart^=jend;
            jend^=jstart;
            jstart^=jend;
        }
        for (j=jstart;j<=jend;j++)
        {
            result.setPixel(j,p1.y()+i,128);
        }
    }
    int ilast=i;
    ydiff=p3.y()-p2.y();
    if (ydiff==0)
        ydiff=1;
    for(i=1;i<= (p3.y()-p2.y()); i++)
    {
        position=(float)i/ydiff;

        jstart=p2.x()+position*(p3.x()-p2.x());

        jend= ceil(p1.x()+((float)ilast/ydiff2)*(p3.x()-p1.x()));
        ilast++;
        if (jstart>jend)
        {
            jstart^=jend;
            jend^=jstart;
            jstart^=jend;
        }
        for (j=jstart;j<=jend;j++)
        {
            result.setPixel(j,p2.y()+i,128);
        }
    }

    //result.setPixel(p1.x(),p1.y(),255);
    //result.setPixel(p2.x(),p2.y(),255);
    //result.setPixel(p3.x(),p3.y(),255);
    return result;
}
