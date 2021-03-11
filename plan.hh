#pragma once

#include "object.hh"
#include "vector.hh"

namespace scene
{
class Plan final : public Object
{
  public:
    Plan(const space::Point3& origin,
         const space::Vector3& normal,
         const std::shared_ptr<TextureMaterial>& texture);
    ~Plan() = default;

    virtual std::optional<float>
    intersect(const space::Ray& ray) const override;

    virtual space::Vector3 normal_get(const space::Point3&) const override;

  private:
    const space::Point3 origin_;
    const space::Vector3 normal_;
};
} // namespace scene