
#pragma once

#include "object.hh"
#include "triangle.hh"
#include "vector.hh"
#include <vector>

namespace scene
{
class Metaball : public Object
{
  public:
    Metaball(const std::vector<space::Point3>& potentials,
             const float step,
             const std::shared_ptr<TextureMaterial>& texture);

    virtual ~Metaball() = default;

    virtual std::optional<space::IntersectionInfo>
    intersect(const space::Ray& ray) const override;

    /* Unreachable */
    virtual space::Vector3
    normal_get(const space::Ray&,
               const space::IntersectionInfo&) const override;

  private:
    struct Cube
    {
        const space::Point3 lower_corner;
        const float length; // Length of the cube
    };

    struct EvaluationZone
    {
        const space::Point3 lower_corner;
        const space::Point3 higher_corner;
        const float step; // step between the cubes
    };

    static constexpr char potential_edge_list_[256][15] = {
#include "edges_list.txt"
    };

  private:
    // Compute every triangle (mesh) of metaball
    void marching_cube();

    // Evaluate potential foreach vertex
    void evaluate_cube(const Cube& cube);

    // Compute index in the edges list according to the given vertex potential
    unsigned char evaluate_vertices(const float vertex_potentials[8]) const;

    // Compute the distance function between a vertex and the potential
    float distance(const space::Point3& vertex,
                   const space::Point3& potential) const;

    // For the constructor, compute the evaluation zone.
    EvaluationZone compute_evaluate_zone(const float step) const;

  private:
    const std::vector<space::Point3> potentials_;

    // Find the furthest and shortest potential
    // then, create the cube
    EvaluationZone eval_zone_;

    // Vector of triangle that are computed during the marching cube algorithm
    // Empty at the beginning and fill of triangles after the call to marching
    // cube
    std::vector<scene::Triangle> triangles_;

    // FIXME: Which value for the threshold
    static constexpr float threshold_ = 1.0f;
};

} // namespace scene