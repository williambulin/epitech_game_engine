
#pragma once

#include "Maths/Vec3.hpp"
#include "Physics/ICollisionShape.hpp"

class Sphere final : public ICollisionShape<Sphere> {
public:
  Vec3 m_center{0.0f, 0.0f, 0.0f};
  float radius{0.0f};

public:
  explicit Sphere(Vec3 center, float radius) noexcept
    : m_center{center},
      radius{radius}
  {
  }

  explicit Sphere(const Sphere &second) noexcept
  : m_center{second.m_center},
    radius{second.radius}
  {
  }

  [[nodiscard]] bool operator==(const Sphere &second) const noexcept final
  {
    return (m_center == second.m_center && radius == second.radius);
  }
};