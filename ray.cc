#include "ray.hh"

namespace space
{
Ray::Ray(const Point3& origin, const Vector3<float>& direction)
    : origin_(origin)
    , direction_(direction) 
{}

Ray::Ray(const Ray& ray)
    : origin_(ray.origin_)
    , direction_(ray.direction_)
{}

std::ostream& operator<<(std::ostream& os, const Ray& ray)
{
    os << "origin:" << ray.origin_ << ", direction:" << ray.direction_;
    return os;
}
} // namespace space