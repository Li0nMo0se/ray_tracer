#pragma once

#include "vector.hh"

namespace color
{
using Color3 = space::Vector<3, unsigned char>;

const Color3 black = Color3({0, 0, 0});
const Color3 white = Color3({255, 255, 255});
}