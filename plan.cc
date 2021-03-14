#include "plan.hh"

namespace scene
{
constexpr float epsilone = 1e-6;

Plan::Plan(const space::Point3& origin,
           const space::Vector3& normal,
           const std::shared_ptr<TextureMaterial>& texture)
    : Object(texture)
    , origin_(origin)
    , normal_(normal.normalized())
{
}

space::Vector3 Plan::normal_get(const space::Point3& intersection) const
{
    // FIXME: normal or -normal according to coming point
    // if dot product is positive, the angle is lower than pi/2.
    // which means they point in the same general direction
    // The normal must be the general opposite direction than the direction
    // if (intersection.dot(normal_) < 0)
    // return -normal_;

    return normal_;
}

std::optional<float> Plan::intersect(const space::Ray& ray) const
{
    // Let's P be the intersection point such as P = O + tD
    // Let's P0 be the origin of the plan
    // Let's n be the normal vector of the plan
    // Intersection if (P - P0).n = 0 (perpendicular)
    // (O + tD - P0).n = 0
    // t(D.n) = (P0 - O).n
    // t = ((P0 - O).n) / (D.n)

    const float denominator = ray.direction_get().dot(normal_);
    // Consider as no intersection
    if (std::abs(denominator) < epsilone)
        return std::nullopt;

    const float numerator = (origin_ - ray.origin_get()).dot(normal_);
    const float t_res = numerator / denominator;
    if (t_res < space::T_MIN)
        return std::nullopt;
    return t_res;
}
} // namespace scene