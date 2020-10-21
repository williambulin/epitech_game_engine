#pragma once

#include <cmath>
#include <optional>

#include "Maths/Vectors.hpp"
#include "Shapes/AABB.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/OBB.hpp"
#include "Transform.hpp"

class ContactPoint final {
public:
  Vector3f localA{0.0f, 0.0f, 0.0f};  // where did the collision occur ...
  Vector3f localB{0.0f, 0.0f, 0.0f};  // in the frame of each object !
  Vector3f normal{0.0f, 0.0f, 0.0f};  // In world space too
  float    penetration;
};

class CollisionInfo final {
  ContactPoint point;

  void AddContactPoint(const Vector3f &localA, const Vector3f &localB, const Vector3f &normal, float p) {
    point.localA = localA;
    point.localB = localB;
    point.normal = normal;
    point.penetration = p;
  }
};

// https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/4collisiondetection/Physics%20-%20Collision%20Detection.pdf
// https://research.ncl.ac.uk/game/mastersdegree/gametechnologies/physicstutorials/5collisionresponse/Physics%20-%20Collision%20Response.pdf
class Collision {
public:
  Collision() {}

  [[nodiscard]] static float SqDistPointAABB(const Vector3<float> &p, const Vector3<float> &aabbMin, const Vector3<float> &aabbMax) noexcept;

  [[nodiscard]] static bool getSeparatingPlane(const Vector3<float> &rPos, const Vector3<float> &plane, const OBB &box1, const OBB &box2) noexcept;

  [[nodiscard]] static bool collide(AABB &firstCollider, Transform modelMatrixFirstCollider, AABB &secondCollider, Transform modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept;

  [[nodiscard]] static bool collide(const Sphere &firstCollider, Transform modelMatrixFirstCollider, const Sphere &secondCollider, Transform modelMatrixSecondcollider, CollisionInfo &collisionInfo) noexcept;

  [[nodiscard]] static bool collide(const Sphere &firstCollider, Transform modelMatrixFirstCollider, AABB &secondCollider, Transform modelMatrixSecondCollider) noexcept;

  [[nodiscard]] static bool collide(const OBB &firstCollider, const OBB &secondCollider) noexcept;
};