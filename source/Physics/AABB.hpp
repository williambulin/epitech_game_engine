
#include "Maths/Vec3.hpp"
#include "Physics/ICollisionSystem.hpp"

class AABB final : public ICollisionSystem<AABB> {
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

  [[nodiscard]] bool overlap(const AABB &second) const noexcept final
  {
    return(m_topRight.x > second.m_bottomLeft.x &&
    m_bottomLeft.x < second.m_topRight.x &&
    m_topRight.y > second.m_bottomLeft.y &&
    m_bottomLeft.y < second.m_topRight.y &&
    m_topRight.z > second.m_bottomLeft.z &&
    m_bottomLeft.z < second.m_topRight.z);
  }

  [[nodiscard]] bool operator==(const AABB &second) const noexcept final
  {
    return (m_bottomLeft == second.m_bottomLeft && m_topRight == second.m_topRight);
  }
};