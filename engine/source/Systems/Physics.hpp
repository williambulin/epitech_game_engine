#pragma once

#include "ECS/System.hpp"
#include "Components/Transform.hpp"
#include "Components/Physics.hpp"

#include "Physics/Shapes/AABB.hpp"
#include "Physics/Shapes/Sphere.hpp"
#include "Physics/Shapes/OBB.hpp"

#include "Maths/Math.hpp"

namespace Systems {
  class Physics;
}

class ContactPoint final {
public:
  Vector3f localA{0.0f, 0.0f, 0.0f};  // where did the collision occur ...
  Vector3f localB{0.0f, 0.0f, 0.0f};  // in the frame of each object !
  Vector3f normal{0.0f, 0.0f, 0.0f};  // In world space too
  float    penetration{0.0f};
};

class CollisionInfo final {
public:
  ContactPoint point{};
  ECS::Entity  firstCollider{};
  ECS::Entity  secondCollider{};
  int          framesLeft{5};

public:
  void addContactPoint(const Vector3f &localA, const Vector3f &localB, const Vector3f &normal, float p) {
    point.localA      = localA;
    point.localB      = localB;
    point.normal      = normal;
    point.penetration = p;
  }
};

// https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/4collisiondetection/Physics%20-%20Collision%20Detection.pdf
// https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/5collisionresponse/Physics%20-%20Collision%20Response.pdf
class Systems::Physics final : public ECS::System<Components::Transform, Components::Physics> {
private:
  std::vector<CollisionInfo> m_collisions;

private:
  void               collisionDections();
  void               collisionResolution();
  void               impulseResolveCollision(CollisionInfo &p) const;
  void               integrateVelocity(float dt);
  [[nodiscard]] bool checkCollisionExists(CollisionInfo existedOne, CollisionInfo toCompare);
  [[nodiscard]] auto getEntityWorldPosition(ICollisionShape &shape, const ml::mat4 &matrix) const -> ml::vec3;

  [[nodiscard]] static bool collide(AABB &firstCollider, const ml::mat4 &modelMatrixFirstCollider, AABB &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept;
  [[nodiscard]] static bool collide(const Sphere &firstCollider, const ml::mat4 &modelMatrixFirstCollider, const Sphere &secondCollider, const ml::mat4 &modelMatrixSecondcollider, CollisionInfo &collisionInfo) noexcept;
  [[nodiscard]] static bool collide(AABB &firstCollider, const ml::mat4 &modelMatrixFirstCollider, const Sphere &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept;
  [[nodiscard]] static bool collide(OBB &firstCollider, const ml::mat4 &modelMatrixFirstCollider, OBB &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept;  // https://gist.github.com/eliasdaler/502b54fcf1b515bcc50360ce874e81bc

public:
  explicit Physics(ECS::Admin &admin) : ECS::System<Components::Transform, Components::Physics>{admin} {}

  void update(float dt, std::uint64_t) final {
    collisionDections();
    collisionResolution();
    integrateVelocity(dt);
  }
};
