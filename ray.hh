#pragma once

#include "vector.hh"
#include <optional>

namespace space
{

constexpr float T_MIN = 1e-5;

class Ray
{
  public:
    Ray(const Point3& origin, const Vector3& direction);

    Ray(const Ray& ray) = default;
    Ray& operator=(const Ray& ray) = default;

    ~Ray() = default;

    friend std::ostream& operator<<(std::ostream& os, const Ray& ray);

    const Point3& origin_get() const;
    const Vector3& direction_get() const;

    float t_get() const;
    const Point3& intersection_get() const;
    bool has_intersected() const;

    static constexpr float intersection_correction_ratio = 1.f / 10.f;
    void auto_intersection_correction(const Vector3& normal);

    // Compute the intersection as well
    void t_set(const float t);

  private:
    Point3 origin_;
    Vector3 direction_;

    std::optional<float> t_ = std::nullopt;
    std::optional<Point3> intersection_ = std::nullopt;
};

std::ostream& operator<<(std::ostream& os, const Ray& ray);

} // namespace space