#pragma once

#include "light.hh"
#include "ray.hh"

namespace scene
{
class PointLight : public Light
{
  public:
    PointLight(const space::Point3& origin, const float intensity)
      : Light(origin, intensity)
    {
    }

    virtual ~PointLight() = default;

    PointLight(const PointLight&) = default;
    virtual PointLight& operator=(const PointLight&) = default;
};
} // namespace scene