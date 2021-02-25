#pragma once

#include "camera.hh"
#include "light.hh"
#include "object.hh"
#include <memory>
#include <vector>

namespace scene
{
class Scene
{
    using objects_t = std::vector<std::shared_ptr<Object>>;
    using lights_t = std::vector<std::shared_ptr<Light>>;

  public:
    Scene(const Camera& camera);
    Scene(const Camera& camera,
          const objects_t& objects,
          const lights_t& lights);

    Scene(const Scene&) = default;
    Scene& operator=(const Scene&) = default;

    void add_object(const std::shared_ptr<Object>& object);
    void add_light(const std::shared_ptr<Light>& light);

  private:
    Camera camera_;

    objects_t objects_;
    lights_t lights_;
};
} // namespace scene