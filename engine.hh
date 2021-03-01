#pragma once

#include <string>

#include "camera.hh"
#include "scene.hh"
#include "color.hh"

namespace rendering
{
class Engine
{
  public:
    static void render(const std::string& filename,
                       const unsigned int width,
                       const unsigned int height,
                       const scene::Scene& scene);

    Engine() = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(const Engine&) = delete;

  private:
    static color::Color3 cast_ray(const space::Ray& ray, const scene::Scene& scene);
    static color::Color3 get_color(const scene::Scene::lights_t& lights, const scene::Object& obj, const space::Point3& intersection);
};
} // namespace rendering