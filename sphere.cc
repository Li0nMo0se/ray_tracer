#include "sphere.hh"

namespace scene
{
Sphere::Sphere(const space::Point3& origin,
               const space::Vector3<float> radius,
               TextureMaterial* const texture)
    : Object(origin, texture)
    , radius_(radius)
{
}

std::optional<space::Point3> Sphere::intersect(const space::Ray& ray) const
{
    // TODO
}

space::Vector3<float> Sphere::get_norm(const space::Point3& p) const
{
    // TODO
}

} // namespace scene