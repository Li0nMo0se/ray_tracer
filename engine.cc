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
    space::Point3 top_left = p - (width / 2 * unit_x * camera.x_axis_) +
                             (height / 2 * unit_y * camera.y_axis_);
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
            // Move to next right pixel
            curr_pixel += unit_x * camera.x_axis_;
            // Ray computation
            const space::Vector3 ray_direction = curr_pixel - camera.origin_;
            color::Color3 pixel_color = cast_ray(space::Ray(camera.origin_, ray_direction), scene);
            im(y, x) = pixel_color;
        }
        // Move to row below
        curr_pixel -= unit_y * camera.y_axis_;
        // Go back to left column
        curr_pixel -= unit_x * width * camera.x_axis_;
    }

    im.save(filename);
}

color::Color3 Engine::cast_ray(const space::Ray& ray, const scene::Scene& scene)
{
    // t such as P = O + tD
    float t = -1.f; // t can't be negative
    space::Point3 intersection;
    std::shared_ptr<scene::Object> intersected_obj = nullptr;
    for (const std::shared_ptr<scene::Object>& obj : scene.objects_)
    {
        const std::optional<float> t_intersection = obj->intersect(ray);
        if (t_intersection && t_intersection.value() < t)
        {
            t = t_intersection.value();
            intersection = ray.origin_get() + t * ray.direction_get();
            intersected_obj = obj;
        }
    }
    if (intersected_obj)
        return intersected_obj->get_texture().get_color(intersection);
    else // No intersection
        return color::black;
}

} // namespace rendering