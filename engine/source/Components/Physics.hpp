#pragma once

#include <memory>
#include <array>

#include "Physics/ICollisionShape.hpp"
#include "Maths/Math.hpp"
#include "Maths/Quaternion.hpp"

namespace Components {
  class Physics;
}

class Components::Physics final {
public:
  std::unique_ptr<ICollisionShape> m_shape{nullptr};

private:
  const float UNIT_MULTIPLIER = 100.0f;
  // const float UNIT_RECIPROCAL = 1.0f / UNIT_MULTIPLIER;

  float m_inverseMass = 1.0f;
  float m_elasticity  = 0.8f;
  float m_friction    = 0.8f;

  bool m_isRigid = false;

  // linear stuff
  ml::vec3 m_linearVelocity{0.0f, 0.0f, 0.0f};
  ml::vec3 m_force{0.0f, 0.0f, 0.0f};

  // angular stuff
  ml::vec3            m_angularVelocity{0.0f, 0.0f, 0.0f};
  ml::vec3            m_torque{0.0f, 0.0f, 0.0f};
  ml::vec3            inverseInertia{0.0f, 0.0f, 0.0f};
  Matrix<float, 3, 3> m_inverseInteriaTensor{std::array<std::array<float, 3>, 3>{
  std::array<float, 3>{1.0f, 0.0f, 0.0f},
  std::array<float, 3>{0.0f, 1.0f, 0.0f},
  std::array<float, 3>{0.0f, 0.0f, 1.0f},
  }};

public:
  explicit Physics(std::unique_ptr<ICollisionShape> shape);

  void clearForces() noexcept;

  ml::vec3 getLinearVelocity() const;
  ml::vec3 getAngularVelocity() const;

  ml::vec3 getTorque() const;
  ml::vec3 getForce() const;

  void  setInverseMass(float invMass);
  float getInverseMass() const;

  void setIsRigid(bool isRigid);
  bool getIsRigid() const;

  void applyAngularImpulse(const ml::vec3 &force);
  void applyLinearImpulse(const ml::vec3 &force);

  void addForce(const ml::vec3 &force);

  void addForceAtPosition(const ml::vec3 &force, const ml::vec3 &position);

  void addTorque(const ml::vec3 &torque);

  void setLinearVelocity(const ml::vec3 &v);
  void setAngularVelocity(const ml::vec3 &v);

  void initCubeInertia();
  void initSphereInertia();

  Matrix<float, 3, 3> getInertiaTensor();
};
