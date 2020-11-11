#include "Physics.hpp"

ml::vec3 Components::Physics::getLinearVelocity() const {
  return m_linearVelocity;
}

ml::vec3 Components::Physics::getAngularVelocity() const {
  return m_angularVelocity;
}

ml::vec3 Components::Physics::getTorque() const {
  return m_torque;
}

ml::vec3 Components::Physics::getForce() const {
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

void Components::Physics::setLinearVelocity(const ml::vec3 &v) {
  m_linearVelocity = v;
}

void Components::Physics::setAngularVelocity(const ml::vec3 &v) {
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

void Components::Physics::applyAngularImpulse(const ml::vec3 &force) {
  m_angularVelocity += m_inverseInteriaTensor * force;
}

void Components::Physics::applyLinearImpulse(const ml::vec3 &force) {
  // m_linearVelocity /= ml::vec3(2.0f, 2.0f, 2.0f);
  m_linearVelocity += force * m_inverseMass;
}

void Components::Physics::addForce(const ml::vec3 &addedForce) {
  m_force += addedForce;
}

void Components::Physics::addForceAtPosition(const ml::vec3 &addedForce, const ml::vec3 &position) {
  // need more function in math
  ml::vec3 localPos = position;  // TODO change it if not working

  m_force += addedForce * Components::Physics::UNIT_MULTIPLIER;
  m_torque += addedForce.cross(localPos);
}

void Components::Physics::addTorque(const ml::vec3 &addedTorque) {
  m_torque += addedTorque;
}

void Components::Physics::clearForces() noexcept {
  m_force  = ml::vec3(0.0f, 0.0f, 0.0f);
  m_torque = ml::vec3(0.0f, 0.0f, 0.0f);
}

void Components::Physics::initCubeInertia() {
  // define get local scale
  ml::vec3 dimensions{1, 1, 1};  //= m_modelMatrix->getLocalScale(); // TODO change here to adjust
  ml::vec3 dimsSqr = dimensions * dimensions;

  inverseInertia.x = (12.0f * m_inverseMass) / (dimsSqr.y + dimsSqr.z);
  inverseInertia.y = (12.0f * m_inverseMass) / (dimsSqr.x + dimsSqr.z);
  inverseInertia.z = (12.0f * m_inverseMass) / (dimsSqr.x + dimsSqr.y);
}

void Components::Physics::initSphereInertia() {
  // get local scale
  float radius = 1;  //= m_modelMatrix->getLocalScale().GetMaxElement(); // TODO change here to adjust
  float i      = 2.5f * m_inverseMass / (radius * radius);

  inverseInertia = ml::vec3(i, i, i);
}
