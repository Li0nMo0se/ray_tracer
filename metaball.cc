#include "metaball.hh"

namespace scene
{
Metaball::Metaball(const std::vector<space::Point3>& potentials,
                   const float step,
                   const std::shared_ptr<TextureMaterial>& texture)
    : Object(texture)
    , potentials_(std::move(potentials))
    , eval_zone_(compute_evaluate_zone(step))
{
    // Build every triangle according to the marching cube algorithm
    marching_cube();
}

void Metaball::pad_eval_zone_border(Metaball::EvaluationZone& eval_zone) const
{
    for (const space::Point3& potential : potentials_)
    {
        for (uint8_t i = 0; i < 3; ++i)
        {
            const space::Vector<1, float> potential_coord(potential[i]);

            // Lower corner
            {
                space::Vector<1, float> lower_coord(eval_zone.lower_corner[i]);
                while (evaluate_potential<1>(potential_coord, lower_coord) >=
                       threshold_)
                {
                    eval_zone.lower_corner[i] -= eval_zone.step;
                    lower_coord[0] = eval_zone.lower_corner[i];
                }
            }

            // Higher corner
            {
                space::Vector<1, float> higher_coord(
                    eval_zone.higher_corner[i]);
                while (evaluate_potential<1>(potential_coord, higher_coord) >=
                       threshold_)
                {
                    eval_zone.higher_corner[i] += eval_zone.step;
                    higher_coord[0] = eval_zone.higher_corner[i];
                }
            }
        }
    }

    // Add more padding to account of cube still being partially in the metaball
    // Due to corner info & epsilone error
    for (uint8_t i = 0; i < 3; ++i)
    {
        eval_zone.lower_corner[i] -= 2 * eval_zone.step;
        eval_zone.higher_corner[i] += 2 * eval_zone.step;
    }
}

Metaball::EvaluationZone Metaball::compute_evaluate_zone(const float step) const
{
    assert(potentials_.size() != 0);
    // Find cube
    space::Point3 min_point(potentials_[0]);
    space::Point3 max_point(potentials_[0]);

    // Find the minimum on the x, y, z axes and the maximum on the x, y, z
    // axes Then, we can have the diagonal of the cube
    for (const space::Point3& potential : potentials_)
    {
        min_point[0] = std::min(potential[0], min_point[0]);
        min_point[1] = std::min(potential[1], min_point[1]);
        min_point[2] = std::min(potential[2], min_point[2]);

        max_point[0] = std::max(potential[0], max_point[0]);
        max_point[1] = std::max(potential[1], max_point[1]);
        max_point[2] = std::max(potential[2], max_point[2]);
    }

    EvaluationZone eval_zone = {min_point, max_point, step};

    // FIXME: Check the padding value
    pad_eval_zone_border(eval_zone);

    return eval_zone;
}

/*
   --------------------
       Marching cube
   --------------------
*/
void Metaball::marching_cube()
{
    for (float z = eval_zone_.lower_corner[2]; z < eval_zone_.higher_corner[2];
         z += eval_zone_.step)
    {
        for (float y = eval_zone_.lower_corner[1];
             y < eval_zone_.higher_corner[1];
             y += eval_zone_.step)
        {
            for (float x = eval_zone_.lower_corner[0];
                 x < eval_zone_.higher_corner[0];
                 x += eval_zone_.step)
            {
                const Cube cube{space::Point3(x, y, z), eval_zone_.step};
                evaluate_cube(cube);
            }
        }
    }
}
/* Compute every triangles in a sub cube
Add it in the vector of triangle

       0-------1
      /|      /|
     / |     / |
    3--|----2  |
    |  4----|--5
    | /     | /
    7-------6

       ----0----
      /|      /|
     3 8     1 |
     / |     / 9
    ---|-2---  |
    |  ---4-|--|
    11 7    10 5
    | /     | /
    ----6----

    y_axis
    |
    |  / z_axis
    | /
    |/
    --------> x_axis
*/

void Metaball::evaluate_cube(const Cube& cube)
{
    // Compute the position of every vertices
    const space::Vector3 x_axis(1.f, 0.f, 0.f);
    const space::Vector3 y_axis(0.f, 1.f, 0.f);
    const space::Vector3 z_axis(0.f, 0.f, 1.f);

    const space::Point3 vertex_7 = cube.lower_corner;
    const space::Point3 vertex_0 = vertex_7 + cube.length * (y_axis + z_axis);
    const space::Point3 vertex_1 =
        vertex_7 + cube.length * (x_axis + y_axis + z_axis);
    const space::Point3 vertex_2 = vertex_7 + cube.length * (x_axis + y_axis);
    const space::Point3 vertex_3 = vertex_7 + cube.length * y_axis;
    const space::Point3 vertex_4 = vertex_7 + cube.length * z_axis;
    const space::Point3 vertex_5 = vertex_7 + cube.length * (x_axis + z_axis);
    const space::Point3 vertex_6 = vertex_7 + cube.length * x_axis;

    const space::Point3 vertices[nb_vertices_cube] = {vertex_0,
                                                      vertex_1,
                                                      vertex_2,
                                                      vertex_3,
                                                      vertex_4,
                                                      vertex_5,
                                                      vertex_6,
                                                      vertex_7};

    // Compute values for every vertex
    std::array<float, nb_vertices_cube> vertices_potentials{0};
    compute_vertices_potentials(vertices, vertices_potentials);

    // Evaluate the activation of the vertices and get the index of the
    // triangle combination
    unsigned char index = evaluate_vertices(vertices_potentials);
    const char* const triangles_vertices = potential_edge_list_[index];

    // Compute the position of the edges according to their identifier (the
    // position computed is the middle of the edge)
    const space::Point3 edge_0 =
        vertex_7 + cube.length * (0.5 * x_axis + y_axis + z_axis);
    const space::Point3 edge_1 =
        vertex_7 + cube.length * (x_axis + y_axis + 0.5 * z_axis);
    const space::Point3 edge_2 =
        vertex_7 + cube.length * (0.5 * x_axis + y_axis);
    const space::Point3 edge_3 =
        vertex_7 + cube.length * (y_axis + 0.5 * z_axis);
    const space::Point3 edge_4 =
        vertex_7 + cube.length * (0.5 * x_axis + z_axis);
    const space::Point3 edge_5 =
        vertex_7 + cube.length * (x_axis + 0.5 * z_axis);
    const space::Point3 edge_6 = vertex_7 + cube.length * 0.5 * x_axis;
    const space::Point3 edge_7 = vertex_7 + cube.length * 0.5 * z_axis;
    const space::Point3 edge_8 =
        vertex_7 + cube.length * (0.5 * y_axis + z_axis);
    const space::Point3 edge_9 =
        vertex_7 + cube.length * (x_axis + 0.5 * y_axis + z_axis);
    const space::Point3 edge_10 =
        vertex_7 + cube.length * (x_axis + 0.5 * y_axis);
    const space::Point3 edge_11 = vertex_7 + cube.length * 0.5 * y_axis;

    const space::Point3 edges[nb_edges_cube] = {edge_0,
                                                edge_1,
                                                edge_2,
                                                edge_3,
                                                edge_4,
                                                edge_5,
                                                edge_6,
                                                edge_7,
                                                edge_8,
                                                edge_9,
                                                edge_10,
                                                edge_11};

    // Create triangles according to the list of triangles vertices
    // Add those new triangles in the list of triangles of the metaball
    for (unsigned char i = 0; i < max_nb_edges && triangles_vertices[i] != -1;
         i += nb_vertices_triangle)
    {
        assert(triangles_vertices[i] >= 0 && triangles_vertices[i] <= 11);
        assert(triangles_vertices[i + 1] >= 0 &&
               triangles_vertices[i + 1] <= 11);
        assert(triangles_vertices[i + 2] >= 0 &&
               triangles_vertices[i + 2] <= 11);

        // Get the vertex of the triangle which are on three edges of the
        // cube triangles_vertices[i] return the number/identifier of the
        // edge Then, get the position of the edge according to its
        // identifier
        const space::Point3& A =
            edges[static_cast<unsigned char>(triangles_vertices[i])];
        const space::Point3& B =
            edges[static_cast<unsigned char>(triangles_vertices[i + 1])];
        const space::Point3& C =
            edges[static_cast<unsigned char>(triangles_vertices[i + 2])];

        triangles_.emplace_back(A, B, C, texture_);
    }
}

void Metaball::compute_vertices_potentials(
    const space::Point3 vertices[nb_vertices_cube],
    std::array<float, nb_vertices_cube>& vertices_potentials) const
{
    for (unsigned short i = 0; i < nb_vertices_cube; i++)
    {
        for (const space::Point3& potential : potentials_)
            vertices_potentials[i] +=
                evaluate_potential<3>(vertices[i], potential);
    }
}

unsigned char Metaball::evaluate_vertices(
    const std::array<float, nb_vertices_cube>& vertex_potentials) const
{
    unsigned char index = 0;
    if (vertex_potentials[0] < threshold_)
        index |= 1;
    if (vertex_potentials[1] < threshold_)
        index |= 2;
    if (vertex_potentials[2] < threshold_)
        index |= 4;
    if (vertex_potentials[3] < threshold_)
        index |= 8;
    if (vertex_potentials[4] < threshold_)
        index |= 16;
    if (vertex_potentials[5] < threshold_)
        index |= 32;
    if (vertex_potentials[6] < threshold_)
        index |= 64;
    if (vertex_potentials[7] < threshold_)
        index |= 128;
    return index;
}
/*
   --------------------
    Intersection area
   --------------------
 */
std::optional<space::IntersectionInfo>
Metaball::intersect(const space::Ray& ray) const
{
    std::optional<space::IntersectionInfo> intersect_global = std::nullopt;
    for (const Triangle& triangle : triangles_)
    {
        std::optional<space::IntersectionInfo> intersect_local =
            triangle.intersect(ray);

        // No intersection, go to next triangle
        if (!intersect_local.has_value())
            continue;

        // Set intersection if first or intersection closer than last one
        const space::IntersectionInfo& intersect_value =
            intersect_local.value();
        if (!intersect_global ||
            intersect_value.t_get() < intersect_global.value().t_get())
        {
            intersect_global = intersect_value;
        }
    }
    return intersect_global;
}

space::Vector3 Metaball::normal_get(const space::Ray&,
                                    const space::IntersectionInfo&) const
{
    __builtin_unreachable();
}

} // namespace scene