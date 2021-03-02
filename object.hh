#pragma once

#include "ray.hh"
#include "texture_material.hh"
#include "vector.hh"
#include <memory>
#include <optional>

namespace scene
{
class Object
{
  public:
    Object(const space::Point3& origin,
           const std::shared_ptr<TextureMaterial>& texture)
        : origin_(origin)
        , texture_(texture)
    {
    }

    virtual ~Object() = default;

    // If no intersection return nullopt
    virtual std::optional<float> intersect(const space::Ray& ray) const = 0;

    virtual space::Vector3 normal_get(const space::Point3& p) const = 0;

    const TextureMaterial& get_texture() const { return *texture_; }

  protected:
    space::Point3 origin_;
    std::shared_ptr<TextureMaterial> texture_;
};
} // namespace scene