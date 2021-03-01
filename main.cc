#include "camera.hh"
#include "image.hh"
#include "point_light.hh"
#include "ray.hh"
#include "uniform_texture.hh"
#include "vector.hh"
#include "sphere.hh"
#include "scene.hh"
#include "engine.hh"

#include <iostream>
#include <string>
#include <sstream>

# define M_PI 3.14159265358979323846  /* pi */

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " filname width height" << std::endl;
        return 1;
    }

    const space::Point3 origin({0, 0, 0});
    const space::Vector3 y_axis({0, 1, 0});
    const space::Vector3 z_axis({0, 0, 1});
    const scene::Camera camera(origin, y_axis, z_axis, 1.f, M_PI / 4.f, M_PI / 4.f);

    // const auto light = std::make_shared<scene::PointLight>(space::Point3({1, 1, 1}), 0.5f); // Useles light for now
    const auto light = std::make_shared<scene::PointLight>(origin, 0.5f); // Useles light for now

    // Object
    const color::Color3 blue({0,0,255});
    const color::Color3 red({255,0,0});
    scene::TextureMaterial* const texture_blue =
        new scene::UniformTexture(blue, 1, 20, 1);
    scene::TextureMaterial* const texture_red =
        new scene::UniformTexture(red, 1, 1, 1);
    // Sphere position
    const space::Point3 sphere1_pos({0, 0, 5});
    const space::Point3 sphere2_pos({0, 1, 6});

    const auto sphere1 = std::make_shared<scene::Sphere>(sphere1_pos, 1, texture_blue);
    const auto sphere2 = std::make_shared<scene::Sphere>(sphere2_pos, 1, texture_red);

    const scene::Scene scene(camera, {sphere1}, {light});

    const std::string filename(argv[1]);

    std::istringstream width_stream(argv[2]);
    unsigned int width;
    width_stream >> width;

    std::istringstream height_stream(argv[3]);
    unsigned int height;
    height_stream >> height;

    rendering::Engine::render(filename, width, height, scene);

    return 0;
}