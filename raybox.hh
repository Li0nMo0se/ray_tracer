#pragma once

#include "object.hh"
#include "vector.hh"

namespace scene
{
class RayBox : public Object
{
  public:
    RayBox(const space::Point3& lower_bound,
           const space::Point3& higher_bound,
           const std::shared_ptr<TextureMaterial>& texture = nullptr);

    virtual std::optional<space::IntersectionInfo>
    intersect(const space::Ray& ray) const override;

    virtual space::Vector3
    normal_get(const space::Ray&,
               const space::IntersectionInfo& intersection) const override;

    inline const space::Point3& lower_bound_get() const { return lower_bound_; }
    inline const space::Point3& higher_bound_get() const
    {
        return higher_bound_;
    };

  private:
    const space::Point3 lower_bound_;
    const space::Point3 higher_bound_;
    const space::Point3 center_;
    const space::Vector3 map_to_unit_box_;
};
} // namespace scene