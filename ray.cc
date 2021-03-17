#include "ray.hh"

namespace space
{

/* Ray */
Ray::Ray(const Point3& origin, const Vector3& direction)
    : origin_(origin)
    , direction_(direction)
{
}

std::ostream& operator<<(std::ostream& os, const Ray& ray)
{
    os << "origin:" << ray.origin_ << ", direction:" << ray.direction_;
    return os;
}

const Point3& Ray::origin_get() const { return origin_; }

const Vector3& Ray::direction_get() const { return direction_; }

} // namespace space