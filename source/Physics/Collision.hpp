#pragma once

#include <cmath>

#include "Maths/Vectors.hpp"
#include "Shapes/AABB.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/OBB.hpp"
#include "Transform.hpp"

class CollisionPointData final {
public:
  Vector3f position{0.0f, 0.0f, 0.0f}; // In world space
  Vector3f surfaceNormal{0.0f, 0.0f, 0.0f}; // In world space too
};

class Collision {
public:
  Collision() {}

  [[nodiscard]] static float SqDistPointAABB(const Vector3<float> &p, const Vector3<float> &aabbMin, const Vector3<float> &aabbMax ) noexcept;

  [[nodiscard]] static bool getSeparatingPlane(const Vector3<float> &rPos, const Vector3<float> &plane, const OBB& box1, const OBB& box2) noexcept;

  [[nodiscard]] static bool collide(AABB &firstCollider, Transform modelMatrixFirstCollider, AABB &secondCollider, Transform modelMatrixSecondCollider) noexcept;

  [[nodiscard]] static bool collide(const Sphere &firstCollider, Transform modelMatrixFirstCollider, const Sphere &secondCollider, Transform modelMatrixSecondcollider) noexcept;

  //https://gamedev.stackexchange.com/questions/156870/how-do-i-implement-a-aabb-sphere-collision
  [[nodiscard]] static bool collide(const Sphere &firstCollider, Transform modelMatrixFirstCollider, AABB &secondCollider, Transform modelMatrixSecondCollider) noexcept;

  //SAT : https://gamedev.stackexchange.com/questions/112883/simple-3d-obb-collision-directx9-c
  [[nodiscard]] static bool collide(const OBB &firstCollider, const OBB &secondCollider) noexcept;
};