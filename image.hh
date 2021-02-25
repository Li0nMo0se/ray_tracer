#pragma once

#include <memory>

namespace image
{
template <typename T>
struct Pixel
{
  public:
    Pixel() // By default it is a black pixel
        : r(0)
        , g(0)
        , b(0)
    {
    }

    T r; // red
    T g; // green
    T b; // blue
};

// TODO templated image
class Image
{
  private:
    using pixel_t = Pixel<unsigned char>;

  public:
    Image(const unsigned int width, const unsigned int height);

    void save(const std::string filename) const;

  private:
    unsigned int width_;
    unsigned int height_;
    std::unique_ptr<pixel_t> data_;
};
} // namespace image