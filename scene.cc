#include "scene.hh"

namespace scene
{
Scene::Scene(const Camera& camera)
    : camera_(camera)
{
}

Scene::Scene(const Camera& camera,
             const objects_t& objects,
             const lights_t& lights)
    : Scene(camera)
{
    objects_ = objects;
    lights_ = lights;
}

void Scene::add_object(const std::shared_ptr<Object>& object)
{
    objects_.push_back(object);
}

void Scene::add_light(const std::shared_ptr<Light>& light)
{
    lights_.push_back(light);
}
} // namespace scene