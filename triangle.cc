#include "triangle.hh"

namespace scene
{
Triangle::Triangle(const space::Point3& A,
                   const space::Point3& B,
                   const space::Point3& C,
                   const std::shared_ptr<TextureMaterial>& texture)
    : Object(texture)
    , A_(A)
    , B_(B)
    , C_(C)
    , normal_(compute_normal())
    , opposite_normal_(-normal_)
{
}

space::Vector3 Triangle::compute_normal() const
{
    const space::Vector3 AB = B_ - A_;
    const space::Vector3 AC = C_ - A_;
    return cross_product(AB, AC).normalized();
}

static inline float det_matrix(const float matrix[3][3])
{
    return matrix[0][0] *
               (matrix[1][1] * matrix[2][2] - matrix[2][1] * matrix[1][2]) -
           matrix[0][1] *
               (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
           matrix[0][2] *
               (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
}

/* Use the Cramer’s Rule */
// Return a Vector3 as the solution of the equation is a matrix shape=(3,1)
// The equation might have infinite or zero solutions, thus return a
// std::optional
static std::optional<space::Vector3> find_solution(const float coeff[3][4])
{
    // Build the 3 Cramer's matrices
    const float d[3][3] = {
        {coeff[0][0], coeff[0][1], coeff[0][2]},
        {coeff[1][0], coeff[1][1], coeff[1][2]},
        {coeff[2][0], coeff[2][1], coeff[2][2]},
    };
    const float d1[3][3] = {
        {coeff[0][3], coeff[0][1], coeff[0][2]},
        {coeff[1][3], coeff[1][1], coeff[1][2]},
        {coeff[2][3], coeff[2][1], coeff[2][2]},
    };
    const float d2[3][3] = {
        {coeff[0][0], coeff[0][3], coeff[0][2]},
        {coeff[1][0], coeff[1][3], coeff[1][2]},
        {coeff[2][0], coeff[2][3], coeff[2][2]},
    };
    const float d3[3][3] = {
        {coeff[0][0], coeff[0][1], coeff[0][3]},
        {coeff[1][0], coeff[1][1], coeff[1][3]},
        {coeff[2][0], coeff[2][1], coeff[2][3]},
    };

    // Calculate determinant of Cramer's matrix
    const float D = det_matrix(d);
    const float D1 = det_matrix(d1);
    const float D2 = det_matrix(d2);
    const float D3 = det_matrix(d3);

    if (D != 0)
        return space::Vector3(D1 / D, D2 / D, D3 / D);
    else // No solution or infinite solution
        return std::nullopt;
}

std::optional<space::IntersectionInfo>
Triangle::intersect(const space::Ray& ray) const
{
    // Solve linera equation using Cramer's rule to find intersection
    float coeff[3][4];
    // Fill coeff
    const space::Vector3& ray_direction = ray.direction_get();
    coeff[0][3] = ray_direction[0];
    coeff[1][3] = ray_direction[1];
    coeff[2][3] = ray_direction[2];

    const space::Point3& O = ray.origin_get();
    const space::Vector3 OA_OB_OC[3] = {A_ - O, B_ - O, C_ - O};
    for (int i = 0; i < 3; ++i)
    {
        coeff[0][i] = OA_OB_OC[i][0];
        coeff[1][i] = OA_OB_OC[i][1];
        coeff[2][i] = OA_OB_OC[i][2];
    }

    const std::optional<space::Vector3> res = find_solution(coeff);
    if (!res) // No solution found
        return std::nullopt;
    const space::Vector3 res_vect = res.value();
    const float alpha = res_vect[0];
    const float beta = res_vect[1];
    const float gamma = res_vect[2];

    // Check same sign
    const bool all_positive = alpha >= 0.f && beta >= 0.f && gamma >= 0.f;
    const bool all_negative = alpha <= 0.f && beta <= 0.f && gamma <= 0.f;

    // Intersection not inside triangle
    if (!all_negative && !all_positive)
        return std::nullopt;

    // Compute OG
    const space::Vector3 OG =
        (alpha * OA_OB_OC[0] + beta * OA_OB_OC[1] + gamma * OA_OB_OC[2]) /
        (alpha + beta + gamma);

    // Find t
    // OG = tD i.e t = OG.x / D.x = OG.y / D.y = OG.z / D.z
    const float t = OG[0] / ray_direction[0];
    if (t < space::T_MIN)
        return std::nullopt;
    return space::IntersectionInfo(t, *this);
}

space::Vector3 Triangle::normal_get(const space::Ray& ray,
                                    const space::IntersectionInfo&) const
{
    // Consider a triangle as a plan (see plan.cc for more details)
    // Return normal or -normal according to the ray
    if (ray.direction_get().dot(normal_) > 0.f)
        return opposite_normal_;

    return normal_;
}
} // namespace scene