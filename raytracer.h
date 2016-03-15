#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <QImage>
#include "model3d.h"

class RayTracer
{
public:
    RayTracer(float x, float y, float z);
    QImage Render();
private:
    Model3D * m_model;
};

#endif // RAYTRACER_H
