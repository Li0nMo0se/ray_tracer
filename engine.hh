#pragma once

#include <optional>
#include <string>

#include "camera.hh"
#include "color.hh"
#include "scene.hh"

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
    static color::Color3 get_object_color(const scene::Scene& scene,
                                          const scene::Object& obj,
                                          const space::Point3& intersection);

    static std::optional<float>
    cast_ray(const space::Ray& ray,
             const std::vector<std::shared_ptr<scene::Object>>& objs,
             std::shared_ptr<scene::Object>& intersected_obj);

    static color::Color3 get_pixel_color(const space::Point3& curr_pixel,
                                         const scene::Scene& scene);

    static bool check_shadow(const scene::Scene& scene,
                             const std::shared_ptr<scene::Light>& light,
                             const space::Point3& intersection);
};
} // namespace rendering