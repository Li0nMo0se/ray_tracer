#pragma once

#include "object.hh"
#include "vector.hh"

namespace scene
{
class Sphere : public Object
{
  public:
    Sphere(const space::Point3& origin,
           const space::Vector3& radius,
           TextureMaterial* const texture);

    // Return positive t value if intersection
    virtual std::optional<float>
    intersect(const space::Ray& ray) const override;

    virtual space::Vector3 get_norm(const space::Point3& intersection) const override;

    space::Vector3 radius_;
};
} //r namespace scene