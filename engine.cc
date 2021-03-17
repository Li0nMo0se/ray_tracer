#include "engine.hh"
#include "image.hh"
#include "vector.hh"

#include <cmath>

namespace rendering
{
void Engine::render(const std::string& filename,
                    const unsigned int resolution_width,
                    const unsigned int resolution_height,
                    const scene::Scene& scene,
                    const unsigned int aliasing_level,
                    const unsigned int reflection_max_depth)
{
    // Create Image
    image::Image im(resolution_width, resolution_height);

    // Find width & height of a pixel
    const scene::Camera& camera = scene.camera_;

    // Compute the height and width of the image in the 3D world
    const float height = std::tan(camera.alpha_) * camera.z_min_ * 2;
    const float width = std::tan(camera.beta_) * camera.z_min_ * 2;

    // Size of a pixel in the 3D world
    const float unit_x = width / resolution_width;
    const float unit_y = height / resolution_height;

    // Find top-left pixel
    // This space::Point3 will be used as a base for vector generation

    // P is the projection of `C` on the image plan
    const space::Point3 p = camera.origin_ + camera.z_axis_ * camera.z_min_;
    // Find the very top left point of the image in the 3D world
    space::Point3 top_left =
        p - (width / 2 * camera.x_axis_) + (height / 2 * camera.y_axis_);
    // Find the center of the top left pixel
    top_left = top_left + (unit_x / 2 * camera.x_axis_) -
               (unit_y / 2 * camera.y_axis_);

    // foreach pixel of the image
    //      Compute the ray from the origin of the camera to the pixel
    //      Find intersections of this ray with every objects of the scene
    //      (Calculate specular & diffuse contribution)

    space::Point3 curr_pixel = top_left;
    for (unsigned int y = 0; y < resolution_height; ++y)
    {
        for (unsigned int x = 0; x < resolution_width; ++x)
        {
            // Ray computation with aliasing
            im(y, x) = get_pixel_color(curr_pixel,
                                       scene,
                                       unit_x,
                                       unit_y,
                                       aliasing_level,
                                       reflection_max_depth);
            // Move to next right pixel
            curr_pixel += unit_x * camera.x_axis_;
        }
        // Move to row below
        curr_pixel -= unit_y * camera.y_axis_;
        // Go back to left column
        curr_pixel -= width * camera.x_axis_;
    }

    im.save(filename);
}

color::Color3 Engine::get_pixel_color(const space::Point3& curr_pixel,
                                      const scene::Scene& scene,
                                      const float unit_x,
                                      const float unit_y,
                                      const unsigned int aliasing_level,
                                      const unsigned int reflection_max_depth)
{
    static constexpr unsigned int reflection_first_depth = 0;
    const scene::Camera& camera = scene.camera_;

    // Aliasing, split the current pixel
    const float aliasing_unit_x = unit_x / aliasing_level;
    const float aliasing_unit_y = unit_y / aliasing_level;
    // top left inner pixel
    space::Vector3 curr_inner_pixel =
        curr_pixel - unit_x / 2 * camera.x_axis_ + unit_y / 2 * camera.y_axis_;
    curr_inner_pixel = curr_inner_pixel + aliasing_unit_x / 2 * camera.x_axis_ -
                       aliasing_unit_y / 2 * camera.y_axis_;
    // Aliasing matrix
    auto matrix_aliasing =
        std::make_unique<color::Color3[]>(aliasing_level * aliasing_level);

    // Get color of every inner pixels
    for (unsigned short y = 0; y < aliasing_level; y++)
    {
        for (unsigned short x = 0; x < aliasing_level; x++)
        {
            const space::Vector3 ray_direction =
                (curr_inner_pixel - camera.origin_).normalized();
            space::Ray ray(camera.origin_, ray_direction);
            matrix_aliasing.get()[y * aliasing_level + x] =
                cast_ray_color(ray,
                               scene,
                               reflection_first_depth,
                               reflection_max_depth);
            curr_inner_pixel += aliasing_unit_x * camera.x_axis_;
        }
        curr_inner_pixel -= aliasing_unit_y * camera.y_axis_;
        curr_inner_pixel -= unit_x * camera.x_axis_;
    }

    // Mean
    color::Color3 color = color::black;
    for (unsigned short i = 0; i < aliasing_level * aliasing_level; i++)
        color += matrix_aliasing.get()[i];
    color /= (aliasing_level * aliasing_level);

    return color;
}

color::Color3 Engine::cast_ray_color(space::Ray& ray,
                                     const scene::Scene& scene,
                                     const unsigned int reflection_curr_depth,
                                     const unsigned int reflection_max_depth)
{
    std::optional<space::IntersectionInfo> intersection = cast_ray(ray, scene);

    if (intersection)
    {
        space::IntersectionInfo& intersection_v = intersection.value();
        const scene::Object& intersected_obj = intersection_v.obj_get();
        // TODO find more elegant way to do this
        intersection_v.auto_intersection_correction(
            intersected_obj.normal_get(ray, intersection_v));
        return get_object_color(scene,
                                ray,
                                intersection_v,
                                reflection_curr_depth,
                                reflection_max_depth);
    }
    return color::black;
}

std::optional<space::IntersectionInfo>
Engine::cast_ray(space::Ray& ray, const scene::Scene& scene)
{
    std::optional<space::IntersectionInfo> closest_intersection = std::nullopt;
    for (const std::shared_ptr<scene::Object>& obj : scene.objects_)
    {
        const std::optional<space::IntersectionInfo> intersection =
            obj->intersect(ray);
        if (intersection &&
            (!closest_intersection || intersection.value().t_get() <
                                          closest_intersection.value().t_get()))
        {
            closest_intersection = intersection;
        }
    }

    // Compute the intersection point if there was an intersection
    if (closest_intersection)
        closest_intersection.value().compute_intersection(ray);
    return closest_intersection;
}

inline float Engine::distance_attenuation(const float distance)
{
    return 1.f / distance;
}

color::Color3
Engine::get_object_color(const scene::Scene& scene,
                         const space::Ray& ray,
                         const space::IntersectionInfo& intersection_info,
                         const unsigned int reflection_curr_depth,
                         const unsigned int reflection_max_depth)
{
    const scene::Object& obj = intersection_info.obj_get();
    // normal of the object at the intersection point
    const space::Vector3& normal = obj.normal_get(ray, intersection_info);
    const space::Vector3& intersection = intersection_info.intersection_get();

    const scene::TextureMaterial& texture = obj.get_texture();
    const float kd = texture.get_kd(intersection);
    const float ks = texture.get_ks(intersection);
    const float ns = texture.get_ns(intersection);
    const color::Color3 obj_color = texture.get_color(intersection);

    color::Color3 color(0.f, 0.f, 0.f);

    // Compute the reflected vector
    const space::Vector3 S =
        intersection - normal * 2 * intersection.dot(normal);

    for (const std::shared_ptr<scene::Light>& light : scene.lights_)
    {
        // Compute shadow (+ normal to avoid intersecting with yourself)
        if (check_shadow(scene, light, intersection))
            continue;

        const space::Vector3 L = light->origin_get() - intersection;
        const float intensity = light->intensity_get();
        // Compute the diffuse light
        const float coeff_diffuse =
            kd * normal.dot(L) * intensity * distance_attenuation(L.length());
        color += obj_color * coeff_diffuse;

        // Compute the specular light
        const float coeff_specular = ks * intensity * powf(S.dot(L), ns);
        if (coeff_specular > 0)
            color += coeff_specular;
    }

    if (reflection_curr_depth < reflection_max_depth && ks > 0.f)
    {
        space::Ray reflected_ray(intersection, S.normalized());
        color += cast_ray_color(reflected_ray,
                                scene,
                                reflection_curr_depth + 1,
                                reflection_max_depth) *
                 ks;
    }

    return color;
}

bool Engine::check_shadow(const scene::Scene& scene,
                          const std::shared_ptr<scene::Light>& light,
                          const space::Point3& intersection)
{
    const space::Vector3 vector_to_light = light->origin_get() - intersection;
    space::Ray ray(intersection, vector_to_light.normalized());

    const float distance_to_light = vector_to_light.length();
    // TODO: Here, we might not want to compute t but only know whether there's
    // a intersection
    const std::optional<space::IntersectionInfo> intersection_info =
        cast_ray(ray, scene);

    if (!intersection_info)
        return false;

    // Is the intersection of the ray between the intersected point and the
    // light
    // t_intersected is the distance between the intersected point and the
    // origin (that's the definition of a ray)
    return intersection_info.value().t_get() < distance_to_light;
}
} // namespace rendering