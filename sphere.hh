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

    virtual std::optional<space::Point3>
    intersect(const space::Ray& ray) const override;

    virtual space::Vector3 get_norm(const space::Point3& p) const override;

  private:
    space::Vector3 radius_;
};
} // namespace scene