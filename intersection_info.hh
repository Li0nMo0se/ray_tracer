#pragma once

#include "ray.hh"
#include "vector.hh"

namespace scene
{
class Object;
}

namespace space
{
class IntersectionInfo
{
  public:
    IntersectionInfo(const float t, const scene::Object& obj)
        : t_(t)
        , obj_(&obj)
    {
    }

    void compute_intersection(const Ray& ray)
    {
        assert(t_.has_value());
        intersection_ = ray.origin_get() + t_.value() * ray.direction_get();
    }

    // FIXME: Instead of using the ratio, move until no intersection with
    // yourself
    static constexpr float intersection_correction_ratio = 0.2f;

    // TODO instead of going to the normal, go back until no intersection with
    // yourself
    void auto_intersection_correction(const Vector3& normal)
    {
        assert(intersection_.has_value());
        intersection_ =
            intersection_.value() + normal * intersection_correction_ratio;
    }

    /* Getters */
    float t_get() const
    {
        assert(t_.has_value());
        return t_.value();
    }

    const scene::Object& obj_get() const
    {
        assert(obj_.has_value());
        return *(obj_.value());
    }

    const Point3& intersection_get() const
    {
        assert(intersection_.has_value());
        return intersection_.value();
    }

  private:
    std::optional<float> t_ = std::nullopt;
    std::optional<const scene::Object*> obj_ = std::nullopt;
    std::optional<Point3> intersection_ = std::nullopt;
};
} // namespace space