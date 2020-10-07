#pragma once

#include <cmath>

#include "Maths/Vec3.hpp"
#include "Shapes/AABB.hpp"
#include "Shapes/Sphere.hpp"

class Collision {
public:
  Collision() {}

  [[nodiscard]] static float SqDistPointAABB(const Vec3 &p, const AABB &b ) noexcept;

  [[nodiscard]] static bool collide(const AABB &first_collider, const AABB &second_collider) noexcept;

  [[nodiscard]] static bool collide(const Sphere &first_collider, const Sphere &second_collider) noexcept;

  //https://gamedev.stackexchange.com/questions/156870/how-do-i-implement-a-aabb-sphere-collision
  [[nodiscard]] static bool collide(const Sphere &first_collider, const AABB &second_collider) noexcept;
};