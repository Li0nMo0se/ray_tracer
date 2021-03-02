#include "sphere.hh"
#include <cmath>

namespace scene
{
Sphere::Sphere(const space::Point3& origin,
               const float radius,
               const std::shared_ptr<TextureMaterial>& texture)
    : Object(origin, texture)
    , radius_(radius)
{
}

static std::optional<float>
solve_quadratic(const float a, const float b, const float c)
{
    float delta = b * b - 4 * a * c;
    if (delta < 0)
        return std::nullopt;
    if (delta == 0)
        return (-b / (2 * a));

    // else delta > 0
    float t0 = (-b + std::sqrt(delta)) / (2 * a);
    float t1 = (-b - std::sqrt(delta)) / (2 * a);

    if (t0 < 0 && t1 < 0)
        return std::nullopt;
    else if (t0 < 0)
        return t1;
    else if (t1 < 0)
        return t0;
    else // t1 and t0 positive
        return t0 < t1 ? t0 : t1;
}

std::optional<float> Sphere::intersect(const space::Ray& ray) const
{
    // P = O + tD
    // If sphere centered at (0, 0, 0)
    // ||P|| = R
    // R^2 = P^2
    // P^2 - R^2 = 0
    // (O + tD)^2 - R^2 = 0
    // O^2 + 2ODt + D^2t^2 - R^2 = 0
    // D^2t^2 + 2ODt + (O^2 - R^2) = 0

    // a = D.D == ||D||^2
    // b = 2* 0.D
    // c = O^2 - R^2

    // If sphere centered at C
    // a = D.D == ||D||^2
    // b = 2* (O - C).D
    // c = (O - C)^2 - R^2

    const space::Point3& ray_origin = ray.origin_get();
    const space::Vector3& ray_direction = ray.direction_get();

    const space::Vector3 L = ray_origin - this->origin_;
    const float a = ray_direction.dot(ray_direction);
    const float b = 2 * ray_direction.dot(L);
    const float c = L.dot(L) - radius_ * radius_;

    const std::optional<float> t_res = solve_quadratic(a, b, c);
    if (t_res && t_res.value() < space::T_MIN)
        return std::nullopt;
    return t_res;
}

space::Vector3 Sphere::normal_get(const space::Point3& intersection) const
{
    // p is the intersection point
    return (intersection - origin_).normalized();
}

} // namespace scene