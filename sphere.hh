#pragma once

#include "object.hh"
#include "vector.hh"

namespace scene
{
class Sphere : public Object
{
  public:
    Sphere(const space::Point3& origin,
           const float radius,
           const std::shared_ptr<TextureMaterial>& texture);

    // Return positive t value if intersection
    virtual std::optional<float>
    intersect(const space::Ray& ray) const override;

    virtual space::Vector3
    normal_get(const space::Ray& intersection) const override;

  private:
    const space::Point3 origin_;
    const float radius_;
};
} // namespace scene