#include "GameObject.hpp"

GameObject::GameObject(std::shared_ptr<ICollisionShape> collider) :
 m_collider(collider),
 m_physicObject(collider, std::make_shared<Transform>(m_modelMatrix))
{
    std::cout << "test in game object " << static_cast<std::underlying_type<ShapeType>::type>(collider->m_shapeType) << std::endl;
} 