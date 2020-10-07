#pragma once

#include "Maths/Vec3.hpp"
#include "Physics/ICollisionShape.hpp"

class OBB final : public ICollisionShape<OBB> {
public:
  Vec3 m_pos{0.0f, 0.0f, 0.0f};
  Vec3 m_axisX{0.0f, 0.0f, 0.0f};
  Vec3 m_axisY{0.0f, 0.0f, 0.0f};
  Vec3 m_axisZ{0.0f, 0.0f, 0.0f};
  Vec3 m_halfSize{0.0f, 0.0f, 0.0f};
public:
  explicit OBB(Vec3 pos, Vec3 axisX, Vec3 axisY, Vec3 axisZ, Vec3 halfSize) noexcept
    : m_pos{pos},
      m_axisX{axisX},
      m_axisY{axisY},
      m_axisZ{axisZ},
      m_halfSize{halfSize}
  {
  }

  explicit OBB(const OBB &second) noexcept
  : m_pos{second.m_pos},
    m_axisX{second.m_axisX},
    m_axisY{second.m_axisY},
    m_axisZ{second.m_axisZ},
    m_halfSize{second.m_halfSize}
  {
  }

  [[nodiscard]] bool operator==(const OBB &second) const noexcept final
  {
    return (m_pos == second.m_pos &&
            m_axisX == second.m_axisX &&
            m_axisY == second.m_axisY &&
            m_axisZ == second.m_axisZ &&
            m_halfSize == second.m_halfSize);
  }
};