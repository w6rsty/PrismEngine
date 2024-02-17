#pragma once

#include "core/core.hpp"
#include "scene/scene.hpp"

#include "toml++/toml.hpp"

namespace prism {

class SceneSerializer {
public:
    SceneSerializer(const Ref<Scene>& scene);

    bool Serialize(const std::string& filepath);

    bool Deserialize(const std::string& filepath);
private:
    void SerializeScene(toml::table& sceneTable); 
    void DeserializeScene(const toml::table& sceneTable);

    void SerializeEntity(toml::array& array, Entity entity);
    void DeserializeEntity(const toml::array& array);
private:
    Ref<Scene> m_Scene;
};

} // namespace prism