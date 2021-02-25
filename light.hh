#pragma once

namespace scene
{
class Light
{
  public:
    Light() = default;
    virtual ~Light() = default;
    Light(const Light&) = default;
    virtual Light& operator=(const Light&) = default;
};
} // namespace scene