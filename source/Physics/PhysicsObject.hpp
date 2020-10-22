#pragma once

#include <memory>
#include <array>

#include "Physics/ICollisionShape.hpp"
#include "Physics/Transform.hpp"

class PhysicsObject {
public:
  explicit PhysicsObject(std::shared_ptr<ICollisionShape> parentCollider, std::shared_ptr<Transform> parentTransform);
	void clearForces() noexcept;
/* 			Vector3 GetLinearVelocity() const {
				return linearVelocity;
			}

			Vector3 GetAngularVelocity() const {
				return angularVelocity;
			}

			Vector3 GetTorque() const {
				return torque;
			}

			Vector3 GetForce() const {
				return force;
			}

			void SetInverseMass(float invMass) {
				inverseMass = invMass;
			}

			float GetInverseMass() const {
				return inverseMass;
			}

			void ApplyAngularImpulse(const Vector3& force);
			void ApplyLinearImpulse(const Vector3& force);
			
			void AddForce(const Vector3& force);

			void AddForceAtPosition(const Vector3& force, const Vector3& position);

			void AddTorque(const Vector3& torque);

			void SetLinearVelocity(const Vector3& v) {
				linearVelocity = v;
			}

			void SetAngularVelocity(const Vector3& v) {
				angularVelocity = v;
			}

			void InitCubeInertia();
			void InitSphereInertia();

			void UpdateInertiaTensor();

			Matrix3 GetInertiaTensor() const {
				return inverseInteriaTensor;
			}
 */
protected:
  const std::shared_ptr<ICollisionShape> m_collider;
  std::shared_ptr<Transform>             m_modelMatrix;

  float m_inverseMass;
  float m_elasticity;
  float m_friction;

  // linear stuff
  Vector3f m_linearVelocity{0.0f, 0.0f, 0.0f};
  Vector3f m_acceleration{0.0f, 0.0f, 0.0f};

  // angular stuff
  Vector3f m_angularVelocity{0.0f, 0.0f, 0.0f};
  Vector3f m_torque{0.0f, 0.0f, 0.0f};
  Vector3f inverseInertia{0.0f, 0.0f, 0.0f};
	Matrix<float, 3, 3> inverseInteriaTensor{
    std::array<std::array<float, 3>, 3> {
    std::array<float, 3>{0.0f, 0.0f, 0.0f},
    std::array<float, 3>{0.0f, 0.0f, 0.0f},
    std::array<float, 3>{0.0f, 0.0f, 0.0f},
    }
  };
};