#pragma once

#include "vector.hh"

namespace scene
{
class Camera
{
  public:
    Camera(space::Vector3<float>& origin,
           space::Vector3<float>& y_axis,
           space::Vector3<float>& z_axis,
           const float z_min,
           const float alpha,
           const float beta);

    Camera(const Camera&) = default;
    Camera& operator=(const Camera&) = default;
    // Copy assignment
  private:
    // Origin of the camera `C`
    space::Vector3<float> origin_;

    // Three axis of the camera
    // Unit vectors
    space::Vector3<float> x_axis_;
    space::Vector3<float> y_axis_;
    space::Vector3<float> z_axis_;

    // Focal distance
    float z_min_;

    // alpha angle
    float alpha_;
    // beta angle
    float beta_;
};

} // namespace scene