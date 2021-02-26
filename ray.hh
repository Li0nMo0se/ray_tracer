#pragma once

#include "vector.hh"

namespace space
{
class Ray
{
  public:
    Ray(const Point3& origin, const Vector3& direction);

    Ray(const Ray& ray) = default;
    Ray& operator=(const Ray& ray) = default;

    ~Ray() = default;

    friend std::ostream& operator<<(std::ostream& os, const Ray& ray);

  private:
    Point3 origin_;
    Vector3 direction_;
};

std::ostream& operator<<(std::ostream& os, const Ray& ray);

} // namespace space