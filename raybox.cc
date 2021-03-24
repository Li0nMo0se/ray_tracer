#include "raybox.hh"

namespace scene
{
RayBox::RayBox(const space::Point3& lower_bound,
               const space::Point3& higher_bound,
               const std::shared_ptr<TextureMaterial>& texture)
    : Object(texture)
    , lower_bound_(lower_bound)
    , higher_bound_(higher_bound)
{
}

std::optional<space::IntersectionInfo>
RayBox::intersect(const space::Ray& ray) const
{
    // The algorithm comes from scratchpixel
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
    const space::Point3& ray_origin = ray.origin_get();
    const space::Vector3& ray_direction = ray.direction_get();

    // FIXME: What to do if the ray_direction is 0 on a coordinate
    if (ray_direction[0] == 0) // parallel to its corresponding plan
    {
    }

    // Compute t_min and t_max on x axis
    float t_min = (lower_bound_[0] - ray_origin[0]) / ray_direction[0];
    float t_max = (higher_bound_[0] - ray_origin[0]) / ray_direction[0];
    // t_min might be greater than t_max according to the ray direction
    // We must have t_min < t_max for the tests above
    if (t_max < t_min)
        std::swap(t_max, t_min);

    // Compute t_min and t_max on y axis
    {
        float t_min_y = (lower_bound_[1] - ray_origin[1]) / ray_direction[1];
        float t_max_y = (higher_bound_[1] - ray_origin[1]) / ray_direction[1];
        if (t_max_y < t_min_y)
            std::swap(t_max_y, t_min_y);

        if (t_min > t_max_y || t_min_y > t_max) // No intersection
            return std::nullopt;

        // Update t according to the x,y plans
        if (t_min_y > t_min)
            t_min = t_min_y;
        if (t_max_y < t_max)
            t_max = t_max_y;
    }

    // Compute t_min and t_max on z axis
    {
        float t_min_z = (lower_bound_[2] - ray_origin[2]) / ray_direction[2];
        float t_max_z = (higher_bound_[2] - ray_origin[2]) / ray_direction[2];
        if (t_max_z < t_min_z)
            std::swap(t_max_z, t_min_z);

        if (t_min > t_max_z || t_max < t_min_z)
            return std::nullopt;

        if (t_min_z > t_min)
            t_min = t_min_z;
        if (t_max_z < t_max)
            t_max = t_max_z;
    }

    // Intersection found
    // Can t_min be greater than t_max here ?

    // FIXME: Check negative t?
    // FIXME: Check t < ESPILON ?
}

space::Vector3
RayBox::normal_get(const space::Ray&,
                   const space::IntersectionInfo& intersection) const
{
    // TODO
    // find on which face the intersection point is
    // Check on the three coordinates
    // Then, return the normal of this face
}

} // namespace scene