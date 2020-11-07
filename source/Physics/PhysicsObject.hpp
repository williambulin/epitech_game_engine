#pragma once

#include <memory>
#include <array>

#include "Physics/ICollisionShape.hpp"
#include "Physics/Transform.hpp"

class PhysicsObject {
public:
	const float UNIT_MULTIPLIER = 100.0f;
	const float UNIT_RECIPROCAL = 1.0f / UNIT_MULTIPLIER;

  explicit PhysicsObject(std::shared_ptr<ICollisionShape> parentCollider, std::shared_ptr<Transform> parentTransform);
	void clearForces() noexcept;
 			Vector3f getLinearVelocity() const {
				return m_linearVelocity;
			}

			Vector3f getAngularVelocity() const {
				return m_angularVelocity;
			}

			Vector3f getTorque() const {
				return m_torque;
			}

			Vector3f getForce() const {
				return m_force;
			}

			void setInverseMass(float invMass) {
				m_inverseMass = invMass;
			}

			float getInverseMass() const {
				return m_inverseMass;
			}

			void applyAngularImpulse(const Vector3f& force);
			void applyLinearImpulse(const Vector3f& force);
			
			void addForce(const Vector3f& force);

			void addForceAtPosition(const Vector3f& force, const Vector3f& position);

			void addTorque(const Vector3f& torque);

			void setLinearVelocity(const Vector3f& v) {
				m_linearVelocity = v;
			}

			void setAngularVelocity(const Vector3f& v) {
				m_angularVelocity = v;
			}

			void initCubeInertia();
			void initSphereInertia();

			void updateInertiaTensor();

			Matrix<float, 3, 3> getInertiaTensor() const {
				return m_inverseInteriaTensor;
			}
private:
  const std::shared_ptr<ICollisionShape> m_collider;
  std::shared_ptr<Transform>             m_modelMatrix;

  float m_inverseMass = 1.0f;
  float m_elasticity = 0.8f;
  float m_friction = 0.8f;

  bool m_isRigid = false;

  // linear stuff
  Vector3f m_linearVelocity{0.0f, 0.0f, 0.0f};
  Vector3f m_force{0.0f, 0.0f, 0.0f};

  // angular stuff
  Vector3f m_angularVelocity{0.0f, 0.0f, 0.0f};
  Vector3f m_torque{0.0f, 0.0f, 0.0f};
  Vector3f inverseInertia{0.0f, 0.0f, 0.0f};
	Matrix<float, 3, 3> m_inverseInteriaTensor{
    std::array<std::array<float, 3>, 3> {
    std::array<float, 3>{0.0f, 0.0f, 0.0f},
    std::array<float, 3>{0.0f, 0.0f, 0.0f},
    std::array<float, 3>{0.0f, 0.0f, 0.0f},
    }
  };
};