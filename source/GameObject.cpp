#include "GameObject.hpp"

GameObject::GameObject(std::shared_ptr<ICollisionShape> collider) :
 m_collider(collider),
 m_physicObject(collider, std::make_shared<Transform>(m_modelMatrix))
{
} 