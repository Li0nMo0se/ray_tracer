#pragma once

#include "vector.hh"

namespace color
{
using Color3 = space::Vector<3, unsigned char>;

const Color3 black = Color3({0, 0, 0});
const Color3 white = Color3({255, 255, 255});

inline Color3 color_multiplication(const Color3& color,
                                   const float coeff)
{
    Color3 new_color = color;
    for (unsigned char& elt : new_color)
    {
        const float res_float = static_cast<float>(elt) * coeff;
        elt = static_cast<unsigned char>(res_float > 255.f ? 255.f : res_float);
    }
    return new_color;
}
}