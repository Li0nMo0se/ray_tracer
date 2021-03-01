#pragma once

#include "vector.hh"

namespace space
{

constexpr float T_MIN = 0.0001f;

class Ray
{
  public:
    Ray(const Point3& origin, const Vector3& direction);

    Ray(const Ray& ray) = default;
    Ray& operator=(const Ray& ray) = default;

    ~Ray() = default;

    friend std::ostream& operator<<(std::ostream& os, const Ray& ray);

    const Point3& origin_get() const;
    const Vector3& direction_get() const;

  private:
    Point3 origin_;
    Vector3 direction_;
};

std::ostream& operator<<(std::ostream& os, const Ray& ray);

} // namespace space