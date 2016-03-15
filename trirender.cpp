#include "trirender.h"
#include <math.h>
#include <string.h>//for memset
#include <iostream>
using namespace std;

typedef struct
{
    long long x;
    long long y;
    long long z;
}Vertice;
const int int_scale=1024;
static unsigned char * image_buffer;
static int image_width;
static int image_height;
int sabs(int a)
{
    if (a<0)
        return -a;
    return a;
}

static void putPixel(long long  x,long long  y, long long  value)
{

    if (x<image_width*int_scale && x>=0 && y<image_height*int_scale && y>=0)
    {
        if (value>(256*int_scale-1))
            value=(256*int_scale-1);
        else if (value<0)
            value=0;
        if (image_buffer[(x/int_scale)+(y/int_scale)*image_width]<(unsigned char)(value/int_scale))
            image_buffer[(x/int_scale)+(y/int_scale)*image_width]=(unsigned char)(value/int_scale);
    }

}

static void drawScanLine(long long  fromx, long long  tox,long long  fromc,long long  toc, long long  y)
{
    /*if (fromx==tox)
    {
        putPixel(tox,y, toc);
        putPixel(tox,y, fromc);
        return;
    }*/
    long long  x,c=fromc;
    x=(fromx+int_scale-1);
    x=x-(x%int_scale);
    for (;x<tox;x+=int_scale)
    {
        c=fromc+(toc-fromc)*(x-fromx)/(tox-fromx);
        putPixel(x,y, c);

    }
}

static void fillBottomFlatTriangle(const Vertice& v1, const Vertice& v2, const Vertice& v3)
{

  long long curx1;
  long long  curx2;
  long long  curc1;
  long long  curc2;
  long long  scanlineY= v1.y;
  scanlineY=(scanlineY+int_scale-1);
  scanlineY=scanlineY-(scanlineY%int_scale);
  if (v1.x>=v3.x)//rounding is toward zero...
  {
      for (; scanlineY < v2.y; scanlineY+=int_scale)
      {
        curx1 = v1.x-((-v2.x + v1.x)*(scanlineY-v1.y) / (v2.y - v1.y));
        curx2 = v1.x-((-v3.x + v1.x)*(scanlineY-v1.y) / (v3.y - v1.y));
        curc1 = v1.z+(v2.z - v1.z)*(scanlineY-v1.y) / (v2.y - v1.y);
        curc2 = v1.z+(v3.z - v1.z)*(scanlineY-v1.y) / (v3.y - v1.y);
        drawScanLine(curx1-1, curx2,curc1, curc2, scanlineY);
      }
  }
  else if (v1.x<=v2.x)
  {
      for (; scanlineY < v2.y; scanlineY+=int_scale)
      {
        curx1 = v1.x+((v2.x - v1.x)*(scanlineY-v1.y) / (v2.y - v1.y));
        curx2 = v1.x+((v3.x - v1.x)*(scanlineY-v1.y) / (v3.y - v1.y));
        curc1 = v1.z+(v2.z - v1.z)*(scanlineY-v1.y) / (v2.y - v1.y);
        curc2 = v1.z+(v3.z - v1.z)*(scanlineY-v1.y) / (v3.y - v1.y);
        drawScanLine(curx1, curx2+1,curc1, curc2, scanlineY);
      }
  }
  else
      for (; scanlineY < v2.y; scanlineY+=int_scale)
      {
        curx1 = v1.x-((-v2.x + v1.x)*(scanlineY-v1.y) / (v2.y - v1.y));
        curx2 = v1.x+((v3.x - v1.x)*(scanlineY-v1.y) / (v3.y - v1.y));
        curc1 = v1.z+(v2.z - v1.z)*(scanlineY-v1.y) / (v2.y - v1.y);
        curc2 = v1.z+(v3.z - v1.z)*(scanlineY-v1.y) / (v3.y - v1.y);
        drawScanLine(curx1-1, curx2+1,curc1, curc2, scanlineY);
      }
}

static void fillTopFlatTriangle(const Vertice& v1, const Vertice& v2, const Vertice& v3)
{

  long long  curx1;
  long long  curx2;
  long long  curc1;
  long long  curc2;
  long long  scanlineY = v3.y-1;
  scanlineY=scanlineY-(scanlineY%int_scale);
  if (v3.x>=v2.x)
  {
      for (; scanlineY >= v1.y; scanlineY-=int_scale)
      {
        curx1 = v3.x-((v3.x - v1.x)*(v3.y-scanlineY) / (v3.y - v1.y));
        curx2 = v3.x-((v3.x - v2.x)*(v3.y-scanlineY) / (v3.y - v2.y));
        curc1 = v3.z-(v3.z - v1.z)*(v3.y-scanlineY) / (v3.y - v1.y);
        curc2 = v3.z-(v3.z - v2.z)*(v3.y-scanlineY) / (v3.y - v2.y);
        drawScanLine(curx1-1, curx2,curc1, curc2, scanlineY);
      }
  }
  else if (v3.x<=v1.x)
  {
      for (; scanlineY >= v1.y; scanlineY-=int_scale)
      {
        curx1 = v3.x+((-v3.x + v1.x)*(v3.y-scanlineY) / (v3.y - v1.y));
        curx2 = v3.x+((-v3.x + v2.x)*(v3.y-scanlineY) / (v3.y - v2.y));
        curc1 = v3.z-(v3.z - v1.z)*(v3.y-scanlineY) / (v3.y - v1.y);
        curc2 = v3.z-(v3.z - v2.z)*(v3.y-scanlineY) / (v3.y - v2.y);
        drawScanLine(curx1, curx2+1,curc1, curc2, scanlineY);
      }
  }
  else
      for (; scanlineY >= v1.y; scanlineY-=int_scale)
      {
        curx1 = v3.x-((v3.x - v1.x)*(v3.y-scanlineY) / (v3.y - v1.y));
        curx2 = v3.x+((-v3.x + v2.x)*(v3.y-scanlineY) / (v3.y - v2.y));
        curc1 = v3.z-(v3.z - v1.z)*(v3.y-scanlineY) / (v3.y - v1.y);
        curc2 = v3.z-(v3.z - v2.z)*(v3.y-scanlineY) / (v3.y - v2.y);
        drawScanLine(curx1-1, curx2+1,curc1, curc2, scanlineY);
      }
}

#define SWAPON(V1,V2,C) do{if(V1->C > V2->C) {temp=V1;V1=V2;V2=temp;}}while(0);

static void drawTriangle(const Vertice* v1,const Vertice* v2,const Vertice* v3)
{
  const Vertice* temp;
  SWAPON(v1,v2,y);
  SWAPON(v1,v3,y);
  SWAPON(v2,v3,y);


  /* here we know that v1->y <= v2->y <= v3->y */
  /* check for trivial case of bottom-flat triangle */

  if (v1->y != v3->y)
  {
      if ( v2->y == v3->y)
      {
            SWAPON(v2,v3,x);
            fillBottomFlatTriangle(*v1, *v2, *v3);
      }
      // check for trivial case of top-flat triangle
      else if (v1->y == v2->y)
      {
        SWAPON(v1,v2,x);
        fillTopFlatTriangle(*v1, *v2, *v3);
      }
      else
      {
        // general case - split the triangle in a topflat and bottom-flat one
        Vertice v4;
        v4.x=(v1->x + ((v3->x - v1->x)*(v2->y - v1->y))/ (v3->y - v1->y));
        v4.y=v2->y;
        v4.z=(v1->z + ((v3->z - v1->z)*(v2->y - v1->y))/ (v3->y - v1->y));
        drawTriangle(v1, v2, &v4);
        drawTriangle(v2, &v4, v3);

      }
  }
  else// all y are equal
  {
      /*SWAPON(v1,v2,x);
      SWAPON(v1,v3,x);
      SWAPON(v2,v3,x);
      drawScanLine(v2->x, v3->x,v2->z, v3->z, v2->y);
      drawScanLine(v1->x, v3->x,v1->z, v3->z, v1->y);
      drawScanLine(v1->x, v2->x,v1->z, v2->z, v1->y);*/

  }
}
static void drawTri(unsigned char *buffer, int w, int h,const long long *points,const unsigned int *triangles, int tri_ind)
{
    unsigned int t[3];
    for (int i=0;i<3;++i)
        t[i]=triangles[tri_ind*3+i];
    const Vertice *v[3];
    for (int i=0;i<3;++i)
    {
        v[i]=(const Vertice*)&points[t[i]*3];
    }
    drawTriangle(v[0],v[1],v[2]);
}
#define DEG2RAD (M_PI/180.0)
#define MAP_MATRIX(I,J) (J*4+I)
static void my_PerspectiveFOV(float fov, float aspect, float near, float far, float* mret) {

    float yScale = 1.0 / tan(DEG2RAD * fov / 2);
    float xScale = yScale / aspect;
    float nearmfar = near - far;
    float m[16] = {
        xScale, 0, 0, 0,
        0, yScale, 0, 0,
        0, 0, (far + near) / nearmfar, -1,
        0, 0, 2*far*near / nearmfar, 0
    };
    memcpy(mret, m, sizeof(float)*16);
}
static void mult_mat(float* m1,float* m2,float* res)
{
    float m[16] = {
        m1[ 0 ]*m2[ 0 ]+m1[ 4 ]*m2[ 1 ]+m1[ 8 ]*m2[ 2 ]+m1[ 12 ]*m2[ 3 ],
        m1[ 1 ]*m2[ 0 ]+m1[ 5 ]*m2[ 1 ]+m1[ 9 ]*m2[ 2 ]+m1[ 13 ]*m2[ 3 ],
        m1[ 2 ]*m2[ 0 ]+m1[ 6 ]*m2[ 1 ]+m1[ 10 ]*m2[ 2 ]+m1[ 14 ]*m2[ 3 ],
        m1[ 3 ]*m2[ 0 ]+m1[ 7 ]*m2[ 1 ]+m1[ 11 ]*m2[ 2 ]+m1[ 15 ]*m2[ 3 ],
        m1[ 0 ]*m2[ 4 ]+m1[ 4 ]*m2[ 5 ]+m1[ 8 ]*m2[ 6 ]+m1[ 12 ]*m2[ 7 ],
        m1[ 1 ]*m2[ 4 ]+m1[ 5 ]*m2[ 5 ]+m1[ 9 ]*m2[ 6 ]+m1[ 13 ]*m2[ 7 ],
        m1[ 2 ]*m2[ 4 ]+m1[ 6 ]*m2[ 5 ]+m1[ 10 ]*m2[ 6 ]+m1[ 14 ]*m2[ 7 ],
        m1[ 3 ]*m2[ 4 ]+m1[ 7 ]*m2[ 5 ]+m1[ 11 ]*m2[ 6 ]+m1[ 15 ]*m2[ 7 ],
        m1[ 0 ]*m2[ 8 ]+m1[ 4 ]*m2[ 9 ]+m1[ 8 ]*m2[ 10 ]+m1[ 12 ]*m2[ 11 ],
        m1[ 1 ]*m2[ 8 ]+m1[ 5 ]*m2[ 9 ]+m1[ 9 ]*m2[ 10 ]+m1[ 13 ]*m2[ 11 ],
        m1[ 2 ]*m2[ 8 ]+m1[ 6 ]*m2[ 9 ]+m1[ 10 ]*m2[ 10 ]+m1[ 14 ]*m2[ 11 ],
        m1[ 3 ]*m2[ 8 ]+m1[ 7 ]*m2[ 9 ]+m1[ 11 ]*m2[ 10 ]+m1[ 15 ]*m2[ 11 ],
        m1[ 0 ]*m2[ 12 ]+m1[ 4 ]*m2[ 13 ]+m1[ 8 ]*m2[ 14 ]+m1[ 12 ]*m2[ 15 ],
        m1[ 1 ]*m2[ 12 ]+m1[ 5 ]*m2[ 13 ]+m1[ 9 ]*m2[ 14 ]+m1[ 13 ]*m2[ 15 ],
        m1[ 2 ]*m2[ 12 ]+m1[ 6 ]*m2[ 13 ]+m1[ 10 ]*m2[ 14 ]+m1[ 14 ]*m2[ 15 ],
        m1[ 3 ]*m2[ 12 ]+m1[ 7 ]*m2[ 13 ]+m1[ 11 ]*m2[ 14 ]+m1[ 15 ]*m2[ 15 ]
    };
    memcpy(res, m, sizeof(float)*16);
}

static void xrotation_mat(float xrot, float* mret) {

    float cxrot=cosf(xrot*DEG2RAD);
    float sxrot=sinf(xrot*DEG2RAD);
    float m[16] = {
        1, 0, 0, 0,
        0, cxrot, sxrot, 0,
        0, -sxrot, cxrot, 0,
        0, 0, 0, 1
    };
    memcpy(mret, m, sizeof(float)*16);
}
static void yrotation_mat(float yrot, float* mret) {

    float cyrot=cosf(yrot*DEG2RAD);
    float syrot=sinf(yrot*DEG2RAD);
    float m[16] = {
        cyrot, 0, -syrot, 0,
        0    , 1, 0     , 0,
        syrot, 0, cyrot , 0,
        0, 0, 0, 1
    };
    memcpy(mret, m, sizeof(float)*16);
}
static void zrotation_mat(float zrot, float* mret) {

    float czrot=cosf(zrot*DEG2RAD);
    float szrot=sinf(zrot*DEG2RAD);
    float m[16] = {
        czrot, szrot, 0, 0,
        -szrot , czrot, 0 , 0,
        0, 0,  1, 0,
        0, 0, 0, 1
    };
    memcpy(mret, m, sizeof(float)*16);
}
static void translation_mat(float x,float y,float z, float* mret) {

    float m[16] = {
        1, 0, 0, 0,
        0 , 1, 0 , 0,
        0, 0,  1, 0,
        x, y, z, 1
    };
    memcpy(mret, m, sizeof(float)*16);
}
static void scale_mat(float sx,float sy,float sz, float* mret) {

    float m[16] = {
        sx, 0, 0, 0,
        0 , sy, 0 , 0,
        0, 0,  sz, 0,
        0, 0, 0, 1
    };
    memcpy(mret, m, sizeof(float)*16);
}

static void mult_mat_vec(float* m , float *v,float *out_v)
{
    float res[4] = {
    m[ 0 ]*v[ 0 ]+m[ 4 ]*v[ 1 ]+m[ 8 ]*v[ 2 ]+m[ 12 ]*v[ 3 ],
    m[ 1 ]*v[ 0 ]+m[ 5 ]*v[ 1 ]+m[ 9 ]*v[ 2 ]+m[ 13 ]*v[ 3 ],
    m[ 2 ]*v[ 0 ]+m[ 6 ]*v[ 1 ]+m[ 10 ]*v[ 2 ]+m[ 14 ]*v[ 3 ],
    m[ 3 ]*v[ 0 ]+m[ 7 ]*v[ 1 ]+m[ 11 ]*v[ 2 ]+m[ 15 ]*v[ 3 ]};
    memcpy(out_v, res, sizeof(float)*4);
}

extern float projMatrix[16];
extern float modelMatrix[16];

void trirender(unsigned char *buffer, int width, int height, const float *points, int np,const unsigned int *triangles, int nt, float xrot, float yrot, float zrot,
               float zoom,float contrast,float xoff,float yoff,float scale)
{
    image_buffer=buffer;
    image_width=width;
    image_height=height;
    memset(buffer,0,width*height);

    long long *i_points=new long long[np*3];
    /*float cxrot=cosf(xrot*DEG2RAD);
    float sxrot=sinf(xrot*DEG2RAD);
    float cyrot=cosf(yrot*DEG2RAD);
    float syrot=sinf(yrot*DEG2RAD);
    float czrot=cosf(zrot*DEG2RAD);
    float szrot=sinf(zrot*DEG2RAD);*/
    float prjmat[16];
    float temp_mat[16];
    float model_mat[16]={1,0,0,0,
                         0,1,0,0,
                         0,0,1,0,
                         0,0,0,1};

    my_PerspectiveFOV(20,(float)width/height,(zoom-contrast),(zoom+contrast),prjmat);

    //float _w,old_val_x;


    //float x,y,z;
    translation_mat(xoff,yoff,-zoom,temp_mat);
    mult_mat(model_mat,temp_mat,model_mat);
    scale_mat(scale,scale,1,temp_mat);
    mult_mat(model_mat,temp_mat,model_mat);
    xrotation_mat(xrot,temp_mat);
    mult_mat(model_mat,temp_mat,model_mat);
    yrotation_mat(yrot,temp_mat);
    mult_mat(model_mat,temp_mat,model_mat);
    zrotation_mat(zrot,temp_mat);
    mult_mat(model_mat,temp_mat,model_mat);

    for (int i=0;i<np;++i)
    {
        float trans_vec[4]={points[i*3+0],points[i*3+1],points[i*3+2],1};
        mult_mat_vec(model_mat,trans_vec,trans_vec);
        mult_mat_vec(prjmat,trans_vec,trans_vec);

        trans_vec[2]=(trans_vec[2]*-128)/trans_vec[3];
        trans_vec[1]=(trans_vec[1]*(height/2))/trans_vec[3];
        trans_vec[0]=(trans_vec[0]*(width/2))/trans_vec[3];

        i_points[i*3]=(long long)(int_scale*(double)trans_vec[0])+width*int_scale/2;
        i_points[i*3+1]=(long long)(int_scale*(double)trans_vec[1])+height*int_scale/2;
        i_points[i*3+2]=(long long)(int_scale*(double)trans_vec[2])+128*int_scale;
    }

    for (int i=0;i<nt;++i)
    {
        drawTri(buffer,width,height,i_points,triangles,i);
    }
    delete [] i_points;
}
