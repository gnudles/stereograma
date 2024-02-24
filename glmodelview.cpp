#include "glmodelview.h"
#include "stereomaker.h"
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <math.h>
#include <stdio.h>
#include <QOpenGLShader>>
#include <QOpenGLShaderProgram>>
#include <QMessageBox>
#include <QProcess>
#include <QCoreApplication>
#include "modeldepthviewer.h"
#include "trirender.h"

GlModelView::GlModelView(QWidget *parent) :
    QOpenGLWidget(parent),m_zoom(500),m_contrast(100)
{
    m_antialias=true;
    m_noShaders=false;
    m_new_width=1024;
    m_new_height=768;
    BasicImageWidget::setBasicImageParent(parent);
}

void GlModelView::initializeGL()
{
    /* TODO
    if (!context()->isValid())
    {
        QMessageBox::warning(this,"No OpenGL Driver","Depth Map generation is still available");
    }*/

    if (QOpenGLShader::hasOpenGLShaders(QOpenGLShader::Vertex,context()) )
    {
        QString declarations= "//uniform mat4 gl_ModelViewMatrix;  uniform mat4 gl_ProjectionMatrix; attribute vec4 gl_Vertex;\n";
        QString codev=       "void main()  {gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"\
                "float zmax=-1.0;"\
                "float zmin=1.0;"\
                "float scale=(zmax-zmin);float offs=-zmin;float z=(gl_Position.z/gl_Position.w); gl_FrontColor = vec4((z+offs)/scale,(z+offs)/scale,(z+offs)/scale,1.0); } ";
        QOpenGLShader shaderv(QOpenGLShader::Vertex);
        bool compile_success=shaderv.compileSourceCode(declarations+codev);
        if (!compile_success)
        {
            compile_success=shaderv.compileSourceCode(codev);
        }
        if (compile_success)
        {
            QOpenGLShaderProgram *program;
            program = new QOpenGLShaderProgram(this);
            program->addShader(&shaderv);
            program->link();
            program->bind();
        }
        else
            m_noShaders=true;
    }
    else
        m_noShaders=true;

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		// This Will Clear The Background Color To Black
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
    // Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    m_rotx=0;
    m_roty=0;
    m_rotz=0;
}
void GlModelView::setModel(Model3D * model)
{
    m_model = model;
}

void GlModelView::setParams(float xoff,float yoff,float zoom,float contrast,float xrot,float yrot,float zrot,float scale)
{
    m_zoom=zoom;
    m_contrast=contrast;
    m_scale=scale;
    m_rotx=xrot;
    m_roty=yrot;
    m_rotz=zrot;
    m_xoffset=xoff;
    m_yoffset=yoff;
    setPerspective();
}

void GlModelView::setZoom(float zoom)
{
    m_zoom=zoom;
    setPerspective();
}
void GlModelView::setScale(float scale)
{
    m_scale=scale;
    setPerspective();
}

void GlModelView::setContrast(float contrast)
{
    m_contrast=contrast;
    setPerspective();
}
typedef struct
{
    unsigned int numPoints;
    unsigned int numTriangles;
    unsigned int raster_width;
    unsigned int raster_height;
    float rotate_x;
    float rotate_y;
    float rotate_z;
    float x_offset;
    float y_offset;
    float zoom;
    float contrast;
} raster_prefs_t;

const QImage* GlModelView::snapShot()
{
    unsigned int out_width=((ModelDepthViewer*)m_parent)->getOutputImageWidth();
    unsigned int out_height=((ModelDepthViewer*)m_parent)->getOutputImageHeight();
    out_width=out_width&0xfffffffc;
    unsigned int numPoints=m_model->numPoints();
    unsigned int numTriangles=m_model->numTriangles();
    const float *points=m_model->pointsData();
    const unsigned int *triangles=m_model->trianglesData();
    unsigned char * buffer=new unsigned char[out_height*out_width];
    float real_scale= pow(2,m_scale/1000);
    trirender(buffer, out_width, out_height, points, numPoints,triangles, numTriangles, m_rotx, m_roty, m_rotz,m_zoom,m_contrast,m_xoffset,m_yoffset,real_scale);
    QByteArray result = QByteArray((const char*)buffer,out_width * out_height);


    imdata=QImage(out_width,out_height,QImage::Format_Indexed8);
    imdata=imdata.convertToFormat(QImage::Format_Indexed8,StereoMaker::getGrayScale());
    memcpy(imdata.bits(),result.data(),result.size());
    imdata=imdata.mirrored();
    delete [] buffer;
    return &imdata;
    /*raster_prefs_t rprefs;
    int outputSize=out_width * out_height * sizeof(GLubyte);
    const float *points=m_model->pointsData();
    const unsigned int *triangles=m_model->trianglesData();
    rprefs.numPoints=m_model->numPoints();
    rprefs.numTriangles=m_model->numTriangles();
    unsigned int out_width=((ModelDepthViewer*)m_parent)->getOutputImageWidth();
    unsigned int out_height=((ModelDepthViewer*)m_parent)->getOutputImageHeight();
    out_width=out_width&0xfffffffc;
    rprefs.raster_width=out_width;
    rprefs.raster_height=out_height;
    rprefs.rotate_x=m_rotx;
    rprefs.rotate_y=m_roty;
    rprefs.rotate_z=m_rotz;
    rprefs.x_offset=m_xoffset;
    rprefs.y_offset=m_yoffset;
    rprefs.zoom=m_zoom;
    rprefs.contrast=m_contrast;

    QProcess renderer;
    QString rasterizer_path=QCoreApplication::applicationDirPath ()+"/m3draster";
    renderer.start(rasterizer_path);
    if (!renderer.waitForStarted())
    {
        QMessageBox::warning(this,"Error","can't open "+rasterizer_path);
        return 0;
    }
    renderer.write((char*)&rprefs,sizeof(raster_prefs_t));
    renderer.waitForBytesWritten();
    int remaining=rprefs.numPoints*3*sizeof(float);
    int bytes_per_write=256*3*sizeof(float);
    int written=0;
    while (remaining>=bytes_per_write)
    {
        renderer.write(((char*)points)+written,bytes_per_write);
        remaining-=bytes_per_write;
        written+=bytes_per_write;
        renderer.waitForBytesWritten();
    }
    if (remaining>0)
    {
        renderer.write(((char*)points)+written,remaining);
        renderer.waitForBytesWritten();
    }

    remaining=rprefs.numTriangles*3*sizeof(unsigned int);
    bytes_per_write=256*3*sizeof(unsigned int);
    written=0;
    while (remaining>=bytes_per_write)
    {
        renderer.write(((char*)triangles)+written,bytes_per_write);
        remaining-=bytes_per_write;
        written+=bytes_per_write;
        renderer.waitForBytesWritten();
    }
    if (remaining>0)
    {
        renderer.write(((char*)triangles)+written,remaining);
        renderer.waitForBytesWritten();
    }

    imdata=QImage();
    if (!renderer.waitForFinished())
        return 0;
    QByteArray result = renderer.readAll();
    int outputSize=out_width * out_height * sizeof(GLubyte);

    if ((unsigned int)result.size()==outputSize)
    {
        imdata=QImage(out_width,out_height,QImage::Format_Indexed8);
        imdata=imdata.convertToFormat(QImage::Format_Indexed8,StereoMaker::getGrayScale());
        memcpy(imdata.bits(),result.data(),result.size());
        imdata.invertPixels();
        imdata=imdata.mirrored();
    }

    return &imdata;*/

}
void print_matrix(int mattype)
{
    //usage:    print_matrix(GL_MODELVIEW_MATRIX);
    //or:   print_matrix(GL_PROJECTION_MATRIX);
    GLfloat matrix[16];
    glGetFloatv (mattype, matrix);
    for (int i=0; i<16; ++i)
    {
        printf("%f ",matrix[(i%4)*4+(i/4)]);
        if (i%4==3)
            printf("\n");
    }
    printf("\n");
    fflush(0);
}

void GlModelView::setPerspective()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(20.0f,(GLfloat)m_new_width/(GLfloat)m_new_height,m_zoom-m_contrast,m_zoom+m_contrast);	// Calculate The Aspect Ratio Of The Window


    glMatrixMode(GL_MODELVIEW);
}

void GlModelView::resizeGL( int width, int height )
{
    unsigned int out_width=((ModelDepthViewer*)m_parent)->getOutputImageWidth();
    unsigned int out_height=((ModelDepthViewer*)m_parent)->getOutputImageHeight();
    setViewport(out_width,out_height);
}

GlModelView::~GlModelView()
{
    if (m_model)
        delete m_model;
}

void GlModelView::setViewport(int width_, int height_)
{
    if (height_==0)										// Prevent A Divide By Zero By
    {
        height_=1;										// Making Height Equal One
    }
    float hratio=(float)height_/height();
    float wratio=(float)width_/width();
    float ratio=qMax(hratio,wratio);
    m_new_width=width_/ratio;
    if (m_new_width>width())
        m_new_width=width();
    m_new_height=height_/ratio;
    if (m_new_height>height())
        m_new_height=height();

    glViewport((width()-m_new_width)/2,(height()-m_new_height)/2,m_new_width,m_new_height);						// Reset The Current Viewport
    setPerspective();
}

void GlModelView::paintGL()
{
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);//clear with white...
    if (m_antialias)
        glEnable(GL_MULTISAMPLE);
    //make black bacground...

    glPushMatrix();
    double large_val=10000;
    glTranslatef(0,0,-m_zoom-m_contrast+0.01);
    glColor3b(255,0,0);
    glBegin(GL_QUADS);
    glVertex2d(large_val,large_val);
    glVertex2d(-large_val,large_val);
    glVertex2d(-large_val,-large_val);
    glVertex2d(large_val,-large_val);
    glEnd();
    glPopMatrix();

    //till here
    if (m_model)
    {
        glPushMatrix();
        glTranslatef(m_xoffset,m_yoffset,-m_zoom);
        float real_scale= pow(2,m_scale/1000);
        glScalef(real_scale,real_scale,1.0);
        glRotatef(m_rotx,1,0,0);
        glRotatef(m_roty,0,1,0);
        glRotatef(m_rotz,0,0,1);
        m_model->DrawGL(m_noShaders);
        glPopMatrix();
    }
    glDisable(GL_MULTISAMPLE);
}
