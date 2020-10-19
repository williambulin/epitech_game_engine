#pragma once

#include "Maths/Vec3.hpp"
#include "Physics/ICollisionShape.hpp"
#include "Physics/Transform.hpp"

// Deduire les deux autres points puis la hitbox
class AABB final : public ICollisionShape<AABB> {
private:
  Vec3 m_min{0.0f, 0.0f, 0.0f};
  Vec3 m_max{0.0f, 0.0f, 0.0f};
  Transform m_oldTransform{};
  std::vector<Vec3> m_pointsCache{};

public:
  explicit AABB(Vec3 min, Vec3 max) noexcept
    : m_min{min},
      m_max{max}
  {
  }

  explicit AABB(const AABB &second) noexcept
  : m_min{second.m_min},
    m_max{second.m_max}
  {
  }

  [[nodiscard]] auto getPoints(Transform transform, bool forceInvalidate = false) -> std::vector<Vec3>;  // Called by collide(...)

  void setMin(const Vec3 &min) noexcept;
  [[nodiscard]] auto getMin() const noexcept -> Vec3;
  void setMax(const Vec3 &max) noexcept;
  [[nodiscard]] auto getMax() const noexcept -> Vec3;

  [[nodiscard]] bool operator==(const AABB &second) const noexcept final;
};