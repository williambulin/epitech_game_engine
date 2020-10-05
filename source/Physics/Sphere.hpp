
#pragma once

#include <cmath>

#include "Maths/Vec3.hpp"
#include "Physics/ICollisionSystem.hpp"

class Sphere final : public ICollisionSystem<Sphere> {
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

  [[nodiscard]] bool overlap(const Sphere &second) const noexcept final
  {
    float distance = sqrt((m_center.x - second.m_center.x) * (m_center.x - second.m_center.x) +
                          (m_center.y - second.m_center.y) * (m_center.y - second.m_center.y) +
                          (m_center.z - second.m_center.z) * (m_center.z - second.m_center.z));
    return distance < (radius + second.radius);
  }

  [[nodiscard]] bool operator==(const Sphere &second) const noexcept final
  {
    return (m_center == second.m_center && radius == second.radius);
  }
};