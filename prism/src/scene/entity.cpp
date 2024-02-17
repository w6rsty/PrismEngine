#include "scene/entity.hpp"

namespace prism {

Entity::Entity(entt::entity handle, Scene* scene)
: m_EntityHandle(handle), 
  m_Scene(scene) {

}

} // namespace prism