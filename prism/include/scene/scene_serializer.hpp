#pragma once

#include "core/core.hpp"
#include "scene/scene.hpp"
namespace prism {

class SceneSerializer {
public:
    SceneSerializer(const Ref<Scene>& scene);

    bool Serialize(const std::string& filepath);

    bool Deserialize(const std::string& filepath);
private:
    Ref<Scene> m_Scene;
};

} // namespace prism