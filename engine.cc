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
    space::Point3 top_left = p - (width / 2 * camera.x_axis_) +
                             (height / 2 * camera.y_axis_);
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
            const space::Vector3 ray_direction = curr_pixel - camera.origin_;
            color::Color3 pixel_color = cast_ray(space::Ray(camera.origin_, ray_direction), scene);
            im(y, x) = pixel_color;
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

color::Color3 Engine::cast_ray(const space::Ray& ray, const scene::Scene& scene)
{
    // t such as P = O + tD
    float t = 0.f;
    space::Point3 intersection;
    std::shared_ptr<scene::Object> intersected_obj = nullptr;
    for (const std::shared_ptr<scene::Object>& obj : scene.objects_)
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
        return get_color(scene.lights_, *intersected_obj, intersection);
    else // No intersection
        return color::black;
}

color::Color3 Engine::get_color(const scene::Scene::lights_t& lights,
                                const scene::Object& obj,
                                const space::Point3& intersection)
{
    // Normale of the object at the intersection point
    const space::Vector3& normale = obj.get_norm(intersection);

    const scene::TextureMaterial& texture = obj.get_texture();
    const float kd = texture.get_kd(intersection);
    const color::Color3 obj_color = texture.get_color(intersection);

    color::Color3 color({0, 0, 0});

    for (const std::shared_ptr<scene::Light>& light : lights)
    {
        // Compute the diffuse light
        const space::Vector3 L = light->origin_get() - intersection;
        const float intensity = light->intensity_get();
        const float coeff = kd * normale.dot(L) * intensity;
        // TODO maybe color in float
        color += obj_color * coeff;

        // Compute the specular light
        // Compute the reflected vector
        const space::Vector3 S = intersection - normale * 2 * intersection.dot(normale);

    }
    return color;
}

} // namespace rendering