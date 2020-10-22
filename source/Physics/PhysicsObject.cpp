#include "Physics/PhysicsObject.hpp"

PhysicsObject::PhysicsObject(std::shared_ptr<ICollisionShape> parentCollider, std::shared_ptr<Transform> parentTransform) :
  m_modelMatrix(parentTransform),
  m_collider(parentCollider)
{
  m_inverseMass = 1.0f;
	m_elasticity	= 0.8f;
	m_friction	= 0.8f;
}

void PhysicsObject::clearForces() {
	m_acceleration	= Vector3f{0.0f, 0.0f, 0.0f};
	m_torque			= Vector3f{0.0f, 0.0f, 0.0f};
}