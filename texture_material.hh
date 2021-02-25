#pragma once

namespace scene
{

using Color3 = space::Vector3<unsigned char>;

class TextureMaterial
{
public:
    TextureMaterial() = default;
    virtual ~TextureMaterial() = default;

    virtual Color3 get_color(const space::Point3&) const = 0;
    virtual float get_kd(const space::Point3&) const = 0;
    virtual float get_ks(const space::Point3&) const = 0;
    virtual float get_ns(const space::Point3&) const = 0;
};
}