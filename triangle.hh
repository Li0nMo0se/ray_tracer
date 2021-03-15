#pragma once

#include "object.hh"
#include "vector.hh"

namespace scene
{
class Triangle : public Object
{
  public:
    Triangle(const space::Point3& A,
             const space::Point3& B,
             const space::Point3& C,
             const std::shared_ptr<TextureMaterial>& texture);

    virtual std::optional<float>
    intersect(const space::Ray& ray) const override;

    virtual space::Vector3
    normal_get(const space::Ray& intersection) const override;

  private:
    const space::Point3 A_;
    const space::Point3 B_;
    const space::Point3 C_;
};
} // namespace scene