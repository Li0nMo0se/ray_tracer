
#pragma once

#include "object.hh"
#include "raybox.hh"
#include "triangle.hh"
#include "vector.hh"
#include <array>
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
        const RayBox raybox;
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

    // static constexpr float min_potential_ = 0.2f;
    // Compute the vertices potential according to the potentials in the
    // metaball
    void compute_vertices_potentials(
        const space::Point3 vertices[nb_vertices_cube],
        std::array<float, nb_vertices_cube>& vertices_potentials) const;

    // Compute index in the edges list according to the given vertex potential
    unsigned char evaluate_vertices(
        const std::array<float, nb_vertices_cube>& vertex_potentials) const;

    // Compute the distance function between a vertex and the potential and
    // deduce the potential value
    template <uint8_t dim>
    float evaluate_potential(const space::Vector<dim, float>& vertex,
                             const space::Vector<dim, float>& potential) const;

    // For the constructor, compute the evaluation zone.
    EvaluationZone compute_evaluate_zone(const float step) const;

    // Add a padding to the evaluation zone to avoid slicing a metaball
    void pad_eval_zone_border(space::Point3& lower,
                              space::Point3& higher,
                              const float step) const;

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

#include "metaball.hxx"