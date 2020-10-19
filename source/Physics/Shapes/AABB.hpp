#pragma once

#include "Maths/Vectors.hpp"
#include "Physics/ICollisionShape.hpp"
#include "Physics/Transform.hpp"

// Deduire les deux autres points puis la hitbox
class AABB final : public ICollisionShape<AABB> {
private:
  Vector3<float> m_min{0.0f, 0.0f, 0.0f};
  Vector3<float> m_max{0.0f, 0.0f, 0.0f};
  Transform m_oldTransform{};
  std::vector<Vector3<float>> m_pointsCache{};

public:
  explicit AABB(Vector3<float> min, Vector3<float> max) noexcept
    : m_min{min},
      m_max{max}
  {
  }

  explicit AABB(const AABB &second) noexcept
  : m_min{second.m_min},
    m_max{second.m_max}
  {
  }

  [[nodiscard]] auto getPoints(Transform transform, bool forceInvalidate = false) -> std::vector<Vector3<float>>;  // Called by collide(...)

  void setMin(const Vector3<float> &min) noexcept;
  [[nodiscard]] auto getMin() const noexcept -> Vector3<float>;
  void setMax(const Vector3<float> &max) noexcept;
  [[nodiscard]] auto getMax() const noexcept -> Vector3<float>;

  [[nodiscard]] bool operator==(const AABB &second) const noexcept final;
};