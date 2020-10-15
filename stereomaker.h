#ifndef STEREOMAKER_H
#define STEREOMAKER_H

#include <QImage>
#include <QProgressBar>
#include "preset.h"

void scaleLine(uchar* big,const uchar *original,int sizeoriginal);
class StereoMaker
{
public:
    StereoMaker();
    QImage render(const QImage & dmap,const QImage & ptrn,Preset *psettings,QProgressBar * qpbar,const QImage * eye_helper_right,const QImage * eye_helper_left,bool show_helper, bool helpers_margin);
    void composeDepth(QImage & depth,QImage & compose, float composeHeight);
    static const QVector<QRgb> & getGrayScale();
private:
    static QVector<QRgb> grayscale;
    int *depthsep;
};

#endif // STEREOMAKER_H
