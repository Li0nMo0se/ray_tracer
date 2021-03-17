#pragma once

#include <optional>
#include <string>

#include "camera.hh"
#include "color.hh"
#include "intersection_info.hh"
#include "scene.hh"

namespace rendering
{
class Engine
{
  public:
    static void render(const std::string& filename,
                       const unsigned int width,
                       const unsigned int height,
                       const scene::Scene& scene,
                       const unsigned int aliasing_level,
                       const unsigned int reflection_max_depth);

    Engine() = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(const Engine&) = delete;

  private:
    static color::Color3
    get_pixel_color(const space::Point3& curr_pixel,
                    const scene::Scene& scene,
                    const float unit_x,
                    const float unit_y,
                    const unsigned int aliasing_level,
                    const unsigned int reflection_max_depth);

    static color::Color3
    cast_ray_color(space::Ray& ray,
                   const scene::Scene& scene,
                   const unsigned int reflection_curr_depth,
                   const unsigned int reflection_max_depth);

    static color::Color3
    get_object_color(const scene::Scene& scene,
                     const space::Ray& ray,
                     const space::IntersectionInfo& intersection_info,
                     const unsigned int reflection_curr_depth,
                     const unsigned int reflection_max_depth);

    static std::optional<space::IntersectionInfo>
    cast_ray(space::Ray& ray, const scene::Scene& scene);

    static bool check_shadow(const scene::Scene& scene,
                             const std::shared_ptr<scene::Light>& light,
                             const space::Point3& intersection);

    inline static float distance_attenuation(const float distance);
};
} // namespace rendering