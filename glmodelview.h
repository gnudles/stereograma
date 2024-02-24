#ifndef GLMODELVIEW_H
#define GLMODELVIEW_H

#include <QOpenGLWidget>
#include <QImage>
#include <model3d.h>
#include "basicimagewidget.h"

class GlModelView : public QOpenGLWidget,public BasicImageWidget
{
    Q_OBJECT
public:
    explicit GlModelView(QWidget *parent = 0);
    ~GlModelView();
    void setViewport(int width,int height);
    void setModel(Model3D * model);
    void setRotationX(float rot){m_rotx=rot;}
    void setRotationY(float rot){m_roty=rot;}
    void setRotationZ(float rot){m_rotz=rot;}
    void setXOffset(float offset){m_xoffset=offset;}
    void setYOffset(float offset){m_yoffset=offset;}
    void setZoom(float zoom);
    void setScale(float scale);
    void setContrast(float contrast);
    void setParams(float xoff,float yoff,float zoom,float contrast,float m_rotx,float m_roty,float m_rotz,float scale);
    const QImage *snapShot();
protected:
    void setPerspective();
    void initializeGL();

    void paintGL();
    void resizeGL( int w, int h );
    /*void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);*/

  protected slots:

  signals:

  public slots:
private:
    Model3D *m_model;
    float m_rotx,m_roty,m_rotz;
    float m_xoffset,m_yoffset;
    float m_zoom;
    float m_contrast;
    float m_scale;
    bool m_noShaders;
    bool m_antialias;
    int m_new_width;
    int m_new_height;
};

#endif // GLMODELVIEW_H
