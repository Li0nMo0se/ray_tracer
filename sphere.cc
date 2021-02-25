#include "sphere.hh"

namespace scene
{
Sphere::Sphere(const space::Point3& origin,
               const space::Vector3& radius,
               const TextureMaterial& texture)
    : origin_(origin)
    , radius_(radius)
    , texture_(texture)
{}

std::optional<space::Point3> Sphere::intersect(const space::Ray& ray) const
{
    // TODO
}

space::Vector3 Sphere::get_norm(const space::Point3& p) const
{
    // TODO
}

} // namespace scene