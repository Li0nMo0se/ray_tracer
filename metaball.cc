#include "metaball.hh"

namespace scene
{
Metaball::Metaball(const std::vector<space::Point3>& potentials,
                   const float step)
    : potentials_(std::move(potentials))
    , eval_zone_(compute_evaluate_zone(step))
{
    // Build every triangle according to the marching cube algorithm
    marching_cube();
}

Metaball::EvaluationZone Metaball::compute_evaluate_zone(const float step) const
{
    assert(potentials_.size() != 0);
    // Find cube
    space::Point3 min_point(potentials_[0]);
    space::Point3 max_point(potentials_[0]);

    // Find the minimum on the x, y, z axes and the maximum on the x, y, z axes
    // Then, we can have the diagonal of the cube
    for (const space::Point3& potential : potentials_)
    {
        min_point[0] = std::min(potential[0], min_point[0]);
        min_point[1] = std::min(potential[1], min_point[1]);
        min_point[2] = std::min(potential[2], min_point[2]);

        max_point[0] = std::max(potential[0], max_point[0]);
        max_point[1] = std::max(potential[1], max_point[1]);
        max_point[2] = std::max(potential[2], max_point[2]);
    }
    return EvaluationZone{min_point, max_point, step};
}

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
     3 8     1 9
     / |     / |
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

    space::Point3 vertices[8] = {vertex_0,
                                 vertex_1,
                                 vertex_2,
                                 vertex_3,
                                 vertex_4,
                                 vertex_5,
                                 vertex_6,
                                 vertex_7};
    float vertices_potentials[8] = {0};
    // Compute values for every potentials
    for (unsigned short i = 0; i < 8; i++)
    {
        for (const space::Point3& potential : potentials_)
            vertices_potentials[i] += distance(vertices[i], potential);
    }
    unsigned char index = evaluate_vertices(vertices_potentials);
    // FIXME: We found the index, find the correct triangles in
    // potential_edge_list and add the triangles from the list for this specific
    // index
}

unsigned char
Metaball::evaluate_vertices(const float vertex_potentials[8]) const
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

float Metaball::distance(const space::Point3& vertex,
                         const space::Point3& potential) const
{
    const float x = potential[0] - vertex[0] * potential[0] - vertex[0];

    const float y = potential[1] - vertex[1] * potential[1] - vertex[1];

    const float z = potential[2] - vertex[2] * potential[2] - vertex[2];

    const float denom = x + y + z;
    if (denom == 0.f) // FIXME
        return 999999999.f;
    return 1 / denom;
}

} // namespace scene