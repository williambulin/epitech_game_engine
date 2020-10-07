#pragma once

#include "Maths/Vec3.hpp"
#include "Physics/ICollisionShape.hpp"

class AABB final : public ICollisionShape<AABB> {
public:
  Vec3 m_topRight{0.0f, 0.0f, 0.0f};
  Vec3 m_bottomLeft{0.0f, 0.0f, 0.0f};

public:
  explicit AABB(Vec3 topRight, Vec3 bottomLeft) noexcept
    : m_topRight{topRight},
      m_bottomLeft{bottomLeft}
  {
  }

  explicit AABB(const AABB &second) noexcept
  : m_topRight{second.m_topRight},
    m_bottomLeft{second.m_bottomLeft}
  {
  }

  [[nodiscard]] bool operator==(const AABB &second) const noexcept final
  {
    return (m_bottomLeft == second.m_bottomLeft && m_topRight == second.m_topRight);
  }
};