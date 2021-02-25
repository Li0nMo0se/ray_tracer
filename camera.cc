#include "camera.hh"

namespace scene
{
Camera::Camera(space::Vector3<float>& origin,
               space::Vector3<float>& y_axis,
               space::Vector3<float>& z_axis,
               const float z_min,
               const float alpha,
               const float beta)
    : origin_(origin)
    , x_axis_(y_axis.cross_product(z_axis))
    , y_axis_(y_axis)
    , z_axis_(z_axis)
    , z_min_(z_min)
    , alpha_(alpha)
    , beta_(beta)
{}
}