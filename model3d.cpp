#include "model3d.h"
#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <qopenglext.h>
#else
#include "qgl.h"
#endif
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "RPly/rply.h"

Model3D::Model3D()
{
    m_triangles=0;
    m_points=0;
    m_colors=0;
    m_nPoints=0;
    m_nTriangles=0;
}

Model3D::~Model3D()
{
    if (m_triangles)
        delete [] m_triangles;
    if (m_points)
        delete [] m_points;
    if (m_colors)
        delete [] m_colors;
}

typedef struct
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
} triple;

void Model3D::LoadObj(const QString & filename)
{
    QByteArray _filename=filename.toUtf8();
    FILE *fp=fopen(_filename.data(),"r");
    if (fp==0)
        return;
    QVector<QVector3D> points;
    QVector<triple> triangles;
    float minX=INT_MAX,maxX=INT_MIN,minY=INT_MAX,maxY=INT_MIN,minZ=INT_MAX,maxZ=INT_MIN;
    //if (fp)
    {
        char buffer[100];
        float vtx[3];
        unsigned int extraface1,extraface2,extraface3;
        int nf;
        triple tf, tf_orig;
        unsigned int dummy;
        while(fgets(buffer,100,fp))
        {
            if(buffer[0]=='v' && buffer[1]!='n' && buffer[1]!='t')
            {
                sscanf(buffer+1,"%f %f %f",vtx,vtx+1,vtx+2);
                points.append(QVector3D(vtx[0],vtx[1],vtx[2]));
                if (vtx[0]<minX)
                    minX=vtx[0];
                else if (vtx[0]>maxX)
                    maxX=vtx[0];
                if (vtx[1]<minY)
                    minY=vtx[1];
                else if (vtx[1]>maxY)
                    maxY=vtx[1];
                if (vtx[2]<minZ)
                    minZ=vtx[2];
                else if (vtx[2]>maxZ)
                    maxZ=vtx[2];
            }
            else if (buffer[0]=='f')
            {

                nf=sscanf(buffer+1,"%u %u %u %u %u %u",&tf.a,&tf.b,&tf.c,&extraface1,&extraface2,&extraface3);
                if (nf<3)
                {
                    nf=sscanf(buffer+1,"%u/%u %u/%u %u/%u %u/%u %u/%u %u/%u",&tf.a,&dummy,&tf.b,&dummy,&tf.c,&dummy,&extraface1,&dummy,&extraface2,&dummy,&extraface3,&dummy);
                    nf/=2;
                    if (nf<3)
                    {
                        nf=sscanf(buffer+1,"%u//%u %u//%u %u//%u %u//%u %u//%u %u//%u",&tf.a,&dummy,&tf.b,&dummy,&tf.c,&dummy,&extraface1,&dummy,&extraface2,&dummy,&extraface3,&dummy);
                        nf/=2;
                        if (nf<3)
                        {
                            nf=sscanf(buffer+1,"%u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u",&tf.a,&dummy,&dummy,&tf.b,&dummy,&dummy,&tf.c,&dummy,&dummy,&extraface1,&dummy,&dummy,&extraface2,&dummy,&dummy,&extraface3,&dummy,&dummy);
                            nf/=3;
                        }
                    }
                }
                if (nf>=3)
                {
                    triangles.append(tf);
                    if (nf>=4)
					{
                        tf_orig = tf;
                        if (nf == 4) //TODO: this is a dummy polygon to triangles conversion. If somebody cares...
                        {
                            tf.b=extraface1;
                            triangles.append(tf);
                        }
                        if (nf == 5)
                        {
                            tf.a=tf_orig.c;
                            tf.b=extraface1;
                            tf.c=extraface2;
                            triangles.append(tf);
                            tf.a=extraface2;
                            tf.b=tf_orig.a;
                            tf.c=tf_orig.c;
                            triangles.append(tf);
                        }
                        if (nf == 6)
                        {
                            tf.a=tf_orig.c;
                            tf.b=extraface1;
                            tf.c=extraface2;
                            triangles.append(tf);
                            tf.a=extraface2;
                            tf.b=tf_orig.a;
                            tf.c=tf_orig.c;
                            triangles.append(tf);
                            tf.a=extraface2;
                            tf.b=tf_orig.a;
                            tf.c=extraface3;
                            triangles.append(tf);
                        }
					}
                }
            }
        }
        fclose(fp);
    }
    float xh=(maxX-minX)/2;
    float yh=(maxY-minY)/2;
    float zh=(maxZ-minZ)/2;
    float xshift = maxX-xh;
    float yshift = maxY-yh;
    float zshift = maxZ-zh;
    float scale = 120.0/sqrt(xh*xh+yh*yh+zh*zh+1);

    m_nPoints=points.size();
    m_nTriangles=triangles.size();
    if (m_triangles)
        delete [] m_triangles;
    if (m_points)
        delete [] m_points;
    m_points=new float[m_nPoints*3];
    m_colors=new unsigned char[m_nPoints*3];
    unsigned char *colorsptr=m_colors;
    float *pointsptr=m_points;
    m_triangles=new unsigned int [m_nTriangles*3];
    unsigned int *trianglesptr=m_triangles;


    for (unsigned int i=0;i<m_nPoints;i++)
    {
        *pointsptr = (float)(points[i].x()-xshift)*scale;
        *colorsptr=(-*pointsptr+135.0);
        pointsptr++;
        colorsptr++;
        *pointsptr = (float)(points[i].y()-yshift)*scale;
        *colorsptr=(*pointsptr+135.0-*(pointsptr-1)/8);
        pointsptr++;
        colorsptr++;
        *pointsptr = (float)(points[i].z()-zshift)*scale;
        *colorsptr=(-*pointsptr+135.0-*(pointsptr-2)/8);
        pointsptr++;
        colorsptr++;
    }

    for (unsigned int i=0;i<m_nTriangles;i++)
    {
        *trianglesptr=triangles[i].a-1;
        trianglesptr++;
        *trianglesptr=triangles[i].b-1;
        trianglesptr++;
        *trianglesptr=triangles[i].c-1;
        trianglesptr++;
    }
}

static int vertex_cb(p_ply_argument argument) {
    //long eol;
    //ply_get_argument_user_data(argument, NULL, &eol);
    float **pointsptr;

    ply_get_argument_user_data(argument,(void**)&pointsptr,0);
    **pointsptr=(float)ply_get_argument_value(argument);
    ++*pointsptr;
    //printf("%g", ply_get_argument_value(argument));
    //if (eol) printf("\n");
    //else printf(" ");
    return 1;
}

static int face_cb(p_ply_argument argument) {
    long length, value_index;
    ply_get_argument_property(argument, NULL, &length, &value_index);
    unsigned int **trianglesptr;

    ply_get_argument_user_data(argument,(void**)&trianglesptr,0);
    if (value_index>=0)
    {
        **trianglesptr=(unsigned int)ply_get_argument_value(argument);
        ++*trianglesptr;
    }

    return 1;
}
void Model3D::LoadPly(const QString &filename)
{

        QByteArray _filename=filename.toUtf8();
        p_ply ply = ply_open(_filename.constData(), NULL, 0, NULL);
        if (!ply) return;
        if (!ply_read_header(ply)) return;
        m_nPoints = ply_set_read_cb(ply, "vertex", "x", 0, NULL, 0);
        m_nTriangles = ply_set_read_cb(ply, "face", "vertex_indices", 0, NULL, 0);
        if (m_triangles)
            delete [] m_triangles;
        if (m_points)
            delete [] m_points;
        m_points=new float[m_nPoints*3];
        m_colors=new unsigned char[m_nPoints*3];
        float* pointsptr=m_points;
        m_triangles=new unsigned int [m_nTriangles*3];
        unsigned int* trianglesptr=m_triangles;
        ply_set_read_cb(ply, "face", "vertex_indices", face_cb, (void*)&trianglesptr, 0);
        ply_set_read_cb(ply, "vertex", "x", vertex_cb, (void*)&pointsptr, 0);
        ply_set_read_cb(ply, "vertex", "y", vertex_cb, (void*)&pointsptr, 0);
        ply_set_read_cb(ply, "vertex", "z", vertex_cb, (void*)&pointsptr, 0);
        //printf("%ld\n%ld\n", nvertices, ntriangles);
        if (!ply_read(ply)) return;
        ply_close(ply);
        normalize();
        return;

}

void Model3D::normalize()
{
    float minX=INT_MAX,maxX=INT_MIN,minY=INT_MAX,maxY=INT_MIN,minZ=INT_MAX,maxZ=INT_MIN;
    float* pointsptr=m_points;
    for (int i=m_nPoints;i>0;--i)
    {
        if (*pointsptr<minX)
            minX=*pointsptr;
        else if (*pointsptr>maxX)
            maxX=*pointsptr;
        ++pointsptr;
        if (*pointsptr<minY)
            minY=*pointsptr;
        else if (*pointsptr>maxY)
            maxY=*pointsptr;
        ++pointsptr;
        if (*pointsptr<minZ)
            minZ=*pointsptr;
        else if (*pointsptr>maxZ)
            maxZ=*pointsptr;
        ++pointsptr;
    }
    float xh=(maxX-minX)/2;
    float yh=(maxY-minY)/2;
    float zh=(maxZ-minZ)/2;
    float xshift = maxX-xh;
    float yshift = maxY-yh;
    float zshift = maxZ-zh;
    float scale = 120.0/sqrt(xh*xh+yh*yh+zh*zh+1);
    pointsptr=m_points;
    unsigned char *colorsptr=m_colors;
    for (int i=m_nPoints;i>0;--i)
    {
        *pointsptr = (float)(*pointsptr-xshift)*scale;
        *colorsptr=(-*pointsptr+135.0);
        pointsptr++;
        colorsptr++;
        *pointsptr = (float)(*pointsptr-yshift)*scale;
        *colorsptr=(*pointsptr+135.0-*(pointsptr-1)/8);
        pointsptr++;
        colorsptr++;
        *pointsptr = (float)(*pointsptr-zshift)*scale;
        *colorsptr=(-*pointsptr+135.0-*(pointsptr-2)/8);
        pointsptr++;
        colorsptr++;
    }
}

void Model3D::DrawGL(bool color)
{
    if (color) glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, m_points);
    if (color) glColorPointer(3, GL_UNSIGNED_BYTE, 0, m_colors);
    glDrawElements(GL_TRIANGLES, m_nTriangles*3, GL_UNSIGNED_INT, m_triangles);
    glDisableClientState(GL_VERTEX_ARRAY);
    if (color) glDisableClientState(GL_COLOR_ARRAY);
}
