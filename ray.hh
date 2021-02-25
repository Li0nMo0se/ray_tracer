#pragma once

#include "vector.hh"

namespace space
{
class Ray
{
  public:
    Ray(const Point3& origin, const Vector3<float>& direction);
    Ray(const Ray& ray);

    ~Ray() = default;

    friend std::ostream& operator<<(std::ostream& os, const Ray& ray);

  private:
    Point3 origin_;
    Vector3<float> direction_;
};

std::ostream& operator<<(std::ostream& os, const Ray& ray);

} // namespace space