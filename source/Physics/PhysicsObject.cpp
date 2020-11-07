#include <Maths/Quaternion.hpp>
#include "Physics/PhysicsObject.hpp"

PhysicsObject::PhysicsObject(std::shared_ptr<ICollisionShape> parentCollider, std::shared_ptr<Transform> parentTransform) :
  m_modelMatrix(parentTransform),
  m_collider(parentCollider)
{
    m_inverseMass = 1.0f;
	m_elasticity  = 0.8f;
	m_friction	  = 0.8f;
	m_isRigid     = false;
}

void PhysicsObject::applyAngularImpulse(const Vector3f& force) {
	m_angularVelocity += m_inverseInteriaTensor * force;
}

void PhysicsObject::applyLinearImpulse(const Vector3f& force) {
	m_linearVelocity /= Vector3f(2.0f, 2.0f, 2.0f); 
	m_linearVelocity += force * m_inverseMass;
}

void PhysicsObject::addForce(const Vector3f& addedForce) {
	m_force += addedForce;
}

void PhysicsObject::addForceAtPosition(const Vector3f& addedForce, const Vector3f& position) {
	//need more function in math
  Vector3f localPos = position; //TODO change it if not working

	m_force += addedForce * PhysicsObject::UNIT_MULTIPLIER;
	m_torque += addedForce.cross(localPos);
}

void PhysicsObject::addTorque(const Vector3f& addedTorque) {
	m_torque += addedTorque;
}

void PhysicsObject::clearForces() noexcept {
	m_force		= Vector3f(0.0f, 0.0f, 0.0f);
	m_torque	= Vector3f(0.0f, 0.0f, 0.0f);
}

void PhysicsObject::initCubeInertia() {
	//define get local scale
  Vector3f dimensions {1, 1, 1};	//= m_modelMatrix->getLocalScale(); // TODO change here to adjust
	Vector3f dimsSqr		= dimensions * dimensions;

	inverseInertia.x = (12.0f * m_inverseMass) / (dimsSqr.y + dimsSqr.z);
	inverseInertia.y = (12.0f * m_inverseMass) / (dimsSqr.x + dimsSqr.z);
	inverseInertia.z = (12.0f * m_inverseMass) / (dimsSqr.x + dimsSqr.y);
}

void PhysicsObject::initSphereInertia() {
	//get local scale
 	float radius = 1;	//= m_modelMatrix->getLocalScale().GetMaxElement(); // TODO change here to adjust
	float i			= 2.5f * m_inverseMass / (radius*radius);

	inverseInertia = Vector3f(i, i, i);
}

void PhysicsObject::updateInertiaTensor() {
	//need to understand
  Quaternion q = Quaternion::fromMatrix(m_modelMatrix->m_modelMatrix);

  Matrix<float, 3, 3> invOrientation	= q.conjugate().toMatrix3();
  Matrix<float, 3, 3> orientation		= q.toMatrix3();

  m_inverseInteriaTensor = orientation * invOrientation;
}
