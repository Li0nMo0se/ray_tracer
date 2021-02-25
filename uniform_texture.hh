#pragma once

#include "texture_material.hh"

namespace scene
{
class UniformTexture : public TextureMaterial
{
  public:
    UniformTexture(const Color3 colors,
                   const float kd,
                   const float ks,
                   const float ns)
        : colors_(colors)
        , kd_(kd)
        , ks_(ks)
        , ns_(ns)
    {
    }

    UniformTexture(const UniformTexture&) = default;
    UniformTexture& operator=(const UniformTexture&) = default;

    Color3 get_color(const space::Point3&) const override { return colors_; }

    float get_kd(const space::Point3&) const override { return kd_; }

    float get_ks(const space::Point3&) const override { return ks_; }

    float get_ns(const space::Point3&) const override { return ns_; }

  private:
    Color3 colors_;
    float kd_;
    float ks_;
    float ns_;
};
} // namespace scene