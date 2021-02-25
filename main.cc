#include "image.hh"
#include "point.hh"
#include "vector.hh"
#include "ray.hh"
#include "camera.hh"
#include "uniform_texture.hh"

#include <iostream>
#include <string>

using namespace space;

int main()
{
    Vector3 v(1, 2, 3);
    v *= 2;
    std::cout << v << std::endl;
    auto v2 = v + v;
    std::cout << v2 << std::endl;
    v += v;
    std::cout << v << std::endl;
    v -= v;
    std::cout << v << std::endl;

    Vector3 v3(1, 2, 3);
    float res = v3.dot(v3);
    std::cout << res << " = " << 14 << std::endl;

    Vector4 v4(1, 2, 3, 4);
    res = v4.dot(v4);
    std::cout << res << " = " << 30 << std::endl;

    Vector3 v5(-3, 4, 3);
    std::cout << v3 << " ^ " << v5 << " = ";
    v3 = v3.cross_product(v5);
    std::cout << v3 << std::endl;

    image::Image im(200, 200);
    im.save("hello.ppn");
}