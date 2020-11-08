#pragma once

#include <memory>

#include "Maths/Vectors.hpp"
#include "Shapes/AABB.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/OBB.hpp"
#include "Transform.hpp"
#include "GameObject.hpp"

class ContactPoint final {
public:
  Vector3f localA{0.0f, 0.0f, 0.0f};  // where did the collision occur ...
  Vector3f localB{0.0f, 0.0f, 0.0f};  // in the frame of each object !
  Vector3f normal{0.0f, 0.0f, 0.0f};  // In world space too
  float    penetration;
};

class CollisionInfo final {
public:
  ContactPoint                point;
  std::shared_ptr<GameObject> firstCollider;
  std::shared_ptr<GameObject> secondCollider;
  int                         framesLeft = 5;

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
class Collision {
public:
  Collision() {}

  [[nodiscard]] static bool collide(AABB &firstCollider, Transform modelMatrixFirstCollider, AABB &secondCollider, Transform modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept;

  [[nodiscard]] static bool collide(const Sphere &firstCollider, Transform modelMatrixFirstCollider, const Sphere &secondCollider, Transform modelMatrixSecondcollider, CollisionInfo &collisionInfo) noexcept;

  [[nodiscard]] static bool collide(AABB &firstCollider, Transform modelMatrixFirstCollider, const Sphere &secondCollider, Transform modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept;

  // https://gist.github.com/eliasdaler/502b54fcf1b515bcc50360ce874e81bc
  [[nodiscard]] static bool collide(OBB &firstCollider, Transform modelMatrixFirstCollider, OBB &secondCollider, Transform modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept;
};
