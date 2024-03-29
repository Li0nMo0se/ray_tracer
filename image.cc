#include "image.hh"

#include <fstream>
#include <limits.h>

// to remove
#include <iostream>

namespace image
{
Image::Image(const unsigned int width, const unsigned int height)
    : width_(width)
    , height_(height)
    // Avoid overflow with the cast
    , data_(new color::Color3[static_cast<size_t>(width_) * height_])
{
    for (unsigned int i = 0; i < width_ * height_; i++)
        data_.get()[i] = color::Color3(0.f, 0.f, 255.f);
}

static unsigned char get_color_with_boundary(const float val)
{
    if (val < 0.f)
        return 0;
    if (val > 255.f)
        return 255;
    return static_cast<unsigned char>(val);
}

void Image::save(const std::string filename) const
{
    std::ofstream of(filename, std::ios_base::out | std::ios_base::binary);

    if (of.fail())
    {
        std::cerr << "Cannot save the image in the file " << filename
                  << std::endl;
        return;
    }

    of << "P6" << std::endl;
    of << width_ << " " << height_ << std::endl;
    unsigned int max_value = std::numeric_limits<unsigned char>::max();
    of << max_value << std::endl;

    for (size_t y = 0; y < height_; y++)
    {
        for (size_t x = 0; x < width_; x++)
        {
            const color::Color3& curr_pixel = data_.get()[y * width_ + x];
            unsigned char r = get_color_with_boundary(curr_pixel[0]);
            unsigned char g = get_color_with_boundary(curr_pixel[1]);
            unsigned char b = get_color_with_boundary(curr_pixel[2]);

            of << r << g << b;
        }
    }

    of.close();
}

color::Color3& Image::operator()(const unsigned int y, const unsigned int x)
{
    return data_.get()[y * width_ + x];
}

} // namespace image