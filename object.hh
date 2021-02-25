#pragma once

#include "vector.hh"
#include <optional>

namespace scene
{
class Object
{
public:
    Object(const space::Point3& origin, const TextureMaterial& texture);
    virtual ~Object() = default;

    // If no intersection return nullopt
    virtual std::optional<space::Point3> intersect(const space::Ray& ray) const = 0;
    virtual space::Vector3 get_norm(const space::Point3& p) const = 0;

    const TextureMaterial& get_texture()
    {
        return texture_;
    }

private:
    space::Point3 origin_;
    TextureMaterial texture_;
}
}