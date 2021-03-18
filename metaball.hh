
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

    /* Unreachable
     *
     * Engine can only get the normal of a triangle of the mesh
     * Never from the metaball itself
     */
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
        space::Point3 lower_corner;
        space::Point3 higher_corner;
        const float step; // step between the cubes
    };

    static constexpr uint8_t max_nb_edges = 15;
    static constexpr uint16_t max_nb_configs = 256;
    static constexpr uint8_t nb_vertices_triangle = 3;
    static constexpr uint8_t nb_vertices_cube = 8;
    static constexpr uint8_t nb_edges_cube = 12;

    static constexpr char potential_edge_list_[max_nb_configs][max_nb_edges] = {
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

    // Add a padding to the evaluation zone to avoid slicing a metaball
    void pad_eval_zone_border(EvaluationZone& eval_zone,
                              const uint8_t pad_coeff) const;

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
    static constexpr float threshold_ = 2.f;
};

} // namespace scene