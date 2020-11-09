#pragma once

#include <memory>
#include <array>

#include "Physics/ICollisionShape.hpp"
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
  Vector3f m_linearVelocity{0.0f, 0.0f, 0.0f};
  Vector3f m_force{0.0f, 0.0f, 0.0f};

  // angular stuff
  Vector3f            m_angularVelocity{0.0f, 0.0f, 0.0f};
  Vector3f            m_torque{0.0f, 0.0f, 0.0f};
  Vector3f            inverseInertia{0.0f, 0.0f, 0.0f};
  Matrix<float, 3, 3> m_inverseInteriaTensor{std::array<std::array<float, 3>, 3>{
  std::array<float, 3>{1.0f, 0.0f, 0.0f},
  std::array<float, 3>{0.0f, 1.0f, 0.0f},
  std::array<float, 3>{0.0f, 0.0f, 1.0f},
  }};

public:
  explicit Physics(std::unique_ptr<ICollisionShape> shape);

  void clearForces() noexcept;

  Vector3f getLinearVelocity() const;
  Vector3f getAngularVelocity() const;

  Vector3f getTorque() const;
  Vector3f getForce() const;

  void  setInverseMass(float invMass);
  float getInverseMass() const;

  void setIsRigid(bool isRigid);
  bool getIsRigid() const;

  void applyAngularImpulse(const Vector3f &force);
  void applyLinearImpulse(const Vector3f &force);

  void addForce(const Vector3f &force);

  void addForceAtPosition(const Vector3f &force, const Vector3f &position);

  void addTorque(const Vector3f &torque);

  void setLinearVelocity(const Vector3f &v);
  void setAngularVelocity(const Vector3f &v);

  void initCubeInertia();
  void initSphereInertia();

  Matrix<float, 3, 3> getInertiaTensor();
};
