#pragma once

#include "object.hh"
#include "vector.hh"

namespace scene
{
class Sphere : public Object
{
  public:
    Sphere(const space::Point3& origin,
           const space::Vector3<float> radius,
           const TextureMaterial& texture);

    virtual std::optional<space::Point3>
    intersect(const space::Ray& ray) const override;

    virtual space::Vector3<float>
    get_norm(const space::Point3& p) const override;

  private:
    space::Vector3<float> radius_;
};
} // namespace scene