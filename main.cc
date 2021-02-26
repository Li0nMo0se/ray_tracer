#include "camera.hh"
#include "image.hh"
#include "point_light.hh"
#include "ray.hh"
#include "uniform_texture.hh"
#include "vector.hh"

#include <iostream>
#include <string>

using namespace space;

int main()
{
    Vector3 v({1, 2, 3});
    v *= 2;
    std::cout << v << std::endl;
    auto v2 = v + v;
    std::cout << v2 << std::endl;
    v += v;
    std::cout << v << std::endl;
    v -= v;
    std::cout << v << std::endl;

    Vector3 v3({1, 2, 3});
    float res = v3.dot(v3);
    std::cout << res << " = " << 14 << std::endl;

    Vector3 v5({-3, 4, 3});
    std::cout << v3 << " ^ " << v5 << " = ";
    v3 = cross_product(v3, v5);
    std::cout << v3 << std::endl;

    v3 = v5;
    std::cout << v3 << " = " << v5 << std::endl;

    v3 = 2 * v5;
    image::Image im(200, 200);
    im.save("hello.ppn");
}