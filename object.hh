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
    Object(const std::shared_ptr<TextureMaterial>& texture)
        : texture_(texture)
    {
    }

    virtual ~Object() = default;

    // If no intersection return nullopt
    virtual std::optional<float> intersect(const space::Ray& ray) const = 0;

    virtual space::Vector3 normal_get(const space::Ray& ray) const = 0;

    const TextureMaterial& get_texture() const { return *texture_; }

  protected:
    std::shared_ptr<TextureMaterial> texture_;
};
} // namespace scene