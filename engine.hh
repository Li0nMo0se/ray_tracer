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
                       const scene::Scene& scene,
                       const unsigned int aliasing_level);

    Engine() = delete;
    Engine& operator=(const Engine&) = delete;
    Engine(const Engine&) = delete;

  private:
    static color::Color3 get_object_color(const scene::Scene& scene,
                                          const scene::Object& obj,
                                          const space::Point3& intersection);

    static color::Color3 cast_ray_color(const space::Ray& ray,
                                        const scene::Scene& scene);

    static std::optional<float>
    cast_ray(const space::Ray& ray,
             const scene::Scene& scene,
             std::shared_ptr<scene::Object>& intersected_obj);

    static color::Color3 get_pixel_color(const space::Point3& curr_pixel,
                                         const scene::Scene& scene,
                                         const float unit_x,
                                         const float unit_y,
                                         const unsigned int aliasing_level);

    static bool check_shadow(const scene::Scene& scene,
                             const std::shared_ptr<scene::Light>& light,
                             const space::Point3& intersection);

    inline static float distance_attenuation(const float distance);

    static constexpr float intersection_correction_ratio = 1.f / 10.f;
    inline static space::Vector3
    auto_intersection_correction(const space::Vector3& intersection,
                                 const space::Vector3& normal);
};
} // namespace rendering