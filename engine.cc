#include "engine.hh"
#include "image.hh"
#include "vector.hh"

#include <cmath>

namespace rendering
{
void Engine::render(const std::string& filename,
                    const unsigned int resolution_width,
                    const unsigned int resolution_height,
                    const scene::Scene& scene)
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
            // Ray computation
            im(y, x) = get_pixel_color(curr_pixel, scene);
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
                                      const scene::Scene& scene)
{
    const scene::Camera& camera = scene.camera_;
    const space::Vector3 ray_direction = curr_pixel - camera.origin_;
    std::shared_ptr<scene::Object> intersected_obj = nullptr;

    const space::Ray ray(camera.origin_, ray_direction);
    std::optional<float> t_intersected =
        cast_ray(ray, scene.objects_, intersected_obj);

    if (t_intersected)
    {
        const space::Point3 intersection =
            ray.origin_get() + t_intersected.value() * ray.direction_get();
        return get_object_color(scene, *intersected_obj, intersection);
    }
    return color::black;
}

std::optional<float>
Engine::cast_ray(const space::Ray& ray,
                 const std::vector<std::shared_ptr<scene::Object>>& objs,
                 std::shared_ptr<scene::Object>& intersected_obj)
{
    intersected_obj = nullptr;
    // t such as P = O + tD
    float t = 0.f;
    space::Point3 intersection;
    for (const std::shared_ptr<scene::Object>& obj : objs)
    {
        const std::optional<float> t_intersection = obj->intersect(ray);
        if (t_intersection && (!intersected_obj || t_intersection.value() < t))
        {
            t = t_intersection.value();
            intersection = ray.origin_get() + t * ray.direction_get();
            intersected_obj = obj;
        }
    }
    if (intersected_obj)
        return t;
    else
        return std::nullopt;
}

color::Color3 Engine::get_object_color(const scene::Scene& scene,
                                       const scene::Object& obj,
                                       const space::Point3& intersection)
{
    // normal of the object at the intersection point
    const space::Vector3& normal = obj.normale_get(intersection);

    const scene::TextureMaterial& texture = obj.get_texture();
    const float kd = texture.get_kd(intersection);
    const float ks = texture.get_ks(intersection);
    const float ns = texture.get_ns(intersection);
    const color::Color3 obj_color = texture.get_color(intersection);

    color::Color3 color({0, 0, 0});

    for (const std::shared_ptr<scene::Light>& light : scene.lights_)
    {
        // Compute shadow
        if (check_shadow(scene, light, intersection))
            return color::black;

        const space::Vector3 L = light->origin_get() - intersection;
        const float intensity = light->intensity_get();
        // Compute the diffuse light
        const float coeff_diffuse = kd * normal.dot(L) * intensity;
        color += obj_color * coeff_diffuse;

        // Compute the specular light
        // Compute the reflected vector
        const space::Vector3 S =
            intersection - normal * 2 * intersection.dot(normal);
        const float coeff_specular = ks * intensity * powf(S.dot(L), ns);
        color += coeff_specular;
    }
    return color;
}

bool Engine::check_shadow(const scene::Scene& scene,
                          const std::shared_ptr<scene::Light>& light,
                          const space::Point3& intersection)
{
    space::Ray ray(intersection,
                   (light->origin_get() - intersection).normalized());

    std::shared_ptr<scene::Object> intersected_obj = nullptr;
    // TODO: Here, we might not want to compute t but only know whether there's
    // a intersection
    const std::optional<float> t_intersected =
        cast_ray(ray, scene.objects_, intersected_obj);

    // Object between the light and the intersection point
    return t_intersected.has_value();
}
} // namespace rendering