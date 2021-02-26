#pragma once

#include "vector.hh"

namespace scene
{

using Color3 = space::Vector<3, unsigned char>;

class TextureMaterial
{
  public:
    TextureMaterial() = default;
    virtual ~TextureMaterial() = default;

    TextureMaterial(const TextureMaterial&) = default;
    virtual TextureMaterial& operator=(const TextureMaterial&) = default;

    virtual Color3 get_color(const space::Point3&) const = 0;
    virtual float get_kd(const space::Point3&) const = 0;
    virtual float get_ks(const space::Point3&) const = 0;
    virtual float get_ns(const space::Point3&) const = 0;
};
} // namespace scene