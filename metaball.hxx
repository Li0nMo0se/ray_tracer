#include "metaball.hh"

namespace scene
{

template <uint8_t dim>
float Metaball::evaluate_potential(
    const space::Vector<dim, float>& vertex,
    const space::Vector<dim, float>& potential) const
{
    float denom = (vertex[0] - potential[0]) * (vertex[0] - potential[0]);
    for (uint8_t i = 1; i < dim; ++i)
        denom += (vertex[i] - potential[i]) * (vertex[i] - potential[i]);

    if (denom <= 1e-6)
    {
        return threshold_;
    }
    return 1 / denom;
}

} // namespace scene