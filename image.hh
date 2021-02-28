#pragma once

#include <memory>
#include "color.hh"

namespace image
{
class Image final
{
  public:
    Image(const unsigned int width, const unsigned int height);
    ~Image() = default;

    void save(const std::string filename) const;

    color::Color3& operator()(const unsigned int y, const unsigned int x);

  private:
    unsigned int width_;
    unsigned int height_;
    std::unique_ptr<color::Color3[]> data_;
};
} // namespace image