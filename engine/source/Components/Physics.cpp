#include "Physics.hpp"

Vector3f Components::Physics::getLinearVelocity() const {
  return m_linearVelocity;
}

Vector3f Components::Physics::getAngularVelocity() const {
  return m_angularVelocity;
}

Vector3f Components::Physics::getTorque() const {
  return m_torque;
}

Vector3f Components::Physics::getForce() const {
  return m_force;
}

void Components::Physics::setInverseMass(float invMass) {
  m_inverseMass = invMass;
}

float Components::Physics::getInverseMass() const {
  return m_inverseMass;
}

void Components::Physics::setIsRigid(bool isRigid) {
  m_isRigid = isRigid;
}

bool Components::Physics::getIsRigid() const {
  return m_isRigid;
}

void Components::Physics::setLinearVelocity(const Vector3f &v) {
  m_linearVelocity = v;
}

void Components::Physics::setAngularVelocity(const Vector3f &v) {
  m_angularVelocity = v;
}

Matrix<float, 3, 3> Components::Physics::getInertiaTensor() {
  return m_inverseInteriaTensor;
}

Components::Physics::Physics(std::unique_ptr<ICollisionShape> shape) : m_shape{std::move(shape)} {
  m_inverseMass = 1.0f;
  m_elasticity  = 0.8f;
  m_friction    = 0.8f;
  m_isRigid     = false;
}

void Components::Physics::applyAngularImpulse(const Vector3f &force) {
  m_angularVelocity += m_inverseInteriaTensor * force;
}

void Components::Physics::applyLinearImpulse(const Vector3f &force) {
  // m_linearVelocity /= Vector3f(2.0f, 2.0f, 2.0f);
  m_linearVelocity += force * m_inverseMass;
}

void Components::Physics::addForce(const Vector3f &addedForce) {
  m_force += addedForce;
}

void Components::Physics::addForceAtPosition(const Vector3f &addedForce, const Vector3f &position) {
  // need more function in math
  Vector3f localPos = position;  // TODO change it if not working

  m_force += addedForce * Components::Physics::UNIT_MULTIPLIER;
  m_torque += addedForce.cross(localPos);
}

void Components::Physics::addTorque(const Vector3f &addedTorque) {
  m_torque += addedTorque;
}

void Components::Physics::clearForces() noexcept {
  m_force  = Vector3f(0.0f, 0.0f, 0.0f);
  m_torque = Vector3f(0.0f, 0.0f, 0.0f);
}

void Components::Physics::initCubeInertia() {
  // define get local scale
  Vector3f dimensions{1, 1, 1};  //= m_modelMatrix->getLocalScale(); // TODO change here to adjust
  Vector3f dimsSqr = dimensions * dimensions;

  inverseInertia.x = (12.0f * m_inverseMass) / (dimsSqr.y + dimsSqr.z);
  inverseInertia.y = (12.0f * m_inverseMass) / (dimsSqr.x + dimsSqr.z);
  inverseInertia.z = (12.0f * m_inverseMass) / (dimsSqr.x + dimsSqr.y);
}

void Components::Physics::initSphereInertia() {
  // get local scale
  float radius = 1;  //= m_modelMatrix->getLocalScale().GetMaxElement(); // TODO change here to adjust
  float i      = 2.5f * m_inverseMass / (radius * radius);

  inverseInertia = Vector3f(i, i, i);
}
