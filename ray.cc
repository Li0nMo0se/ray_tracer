#include "ray.hh"

namespace space
{
Ray::Ray(const Point3& origin, const Vector3<float>& direction)
    : origin_(origin)
    , direction_(direction)
{
}

std::ostream& operator<<(std::ostream& os, const Ray& ray)
{
    os << "origin:" << ray.origin_ << ", direction:" << ray.direction_;
    return os;
}
} // namespace space