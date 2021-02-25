#pragma once

#include "object.hh"

namespace scene
{
class Sphere : public Object
{
public:
    Sphere(const space::Point3& origin,
           const space::Vector3& radius,
           const TextureMaterial& texture);

    virtual std::optional<space::Point3> intersect(const space::Ray& ray) const override;
    virtual Vector3 get_norm(const space::Point3& p) const override;

private:
    float radius_;
}
} // namespace scene