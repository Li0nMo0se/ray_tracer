#include "ray.hh"

namespace space
{
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

const Point3& Ray::origin_get() const
{
    return origin_;
}

const Vector3& Ray::direction_get() const
{
    return direction_;
}

float Ray::t_get() const
{
    assert(t_.has_value());
    return t_.value();
}

const Point3& Ray::intersection_get() const
{
    assert(intersection_.has_value());
    return intersection_.value();
}

// Compute the intersection as well
void Ray::t_set(const float t)
{
    t_ = t;
    intersection_ = origin_ + t_.value() * direction_;
}

void Ray::auto_intersection_correction(const space::Vector3& normal)
{
    intersection_ =  intersection_.value() + normal * intersection_correction_ratio;
}

bool Ray::has_intersected() const
{
    return t_.has_value();
}

} // namespace space