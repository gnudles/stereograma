#ifndef TRIRENDER_H
#define TRIRENDER_H

void trirender(unsigned char *buffer, int w, int h, const float *points, int np, const unsigned int *triangles, int nt, float xrot, float yrot, float zrot, float zoom, float contrast, float xoff, float yoff, float scale);

#endif // TRIRENDER_H
