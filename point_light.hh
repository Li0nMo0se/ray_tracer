#pragma once

#include "light.hh"
#include "ray.hh"

namespace scene
{
class PointLight : public Light
{
  public:
    PointLight(const space::Ray& ray)
        : ray_(ray)
    {
    }

    ~PointLight() = default;

  protected:
    space::Ray ray_;
};
} // namespace scene