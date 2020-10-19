#pragma once

#include <cmath>

#include "Maths/Vectors.hpp"
#include "Shapes/AABB.hpp"
#include "Shapes/Sphere.hpp"
#include "Shapes/OBB.hpp"

class Collision {
public:
  Collision() {}

  [[nodiscard]] static float SqDistPointAABB(const Vector3<float> &p, const AABB &b ) noexcept;

  [[nodiscard]] static bool getSeparatingPlane(const Vector3<float> &rPos, const Vector3<float> &plane, const OBB& box1, const OBB& box2) noexcept;

  [[nodiscard]] static bool collide(const AABB &firstCollider, const AABB &secondCollider) noexcept;

  [[nodiscard]] static bool collide(const Sphere &firstCollider, const Sphere &secondCollider) noexcept;

  //https://gamedev.stackexchange.com/questions/156870/how-do-i-implement-a-aabb-sphere-collision
  [[nodiscard]] static bool collide(const Sphere &firstCollider, const AABB &secondCollider) noexcept;

  //SAT : https://gamedev.stackexchange.com/questions/112883/simple-3d-obb-collision-directx9-c
  [[nodiscard]] static bool collide(const OBB &firstCollider, const OBB &secondCollider) noexcept;
};