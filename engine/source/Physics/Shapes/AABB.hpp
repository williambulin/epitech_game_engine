#pragma once

#include "Maths/Vectors.hpp"
#include "Physics/ICollisionShape.hpp"
#include "Components/Transform.hpp"

// Deduire les deux autres points puis la hitbox
class AABB final : public ICollisionShape {
public:
  explicit AABB(const Vector3f &min, const Vector3f &max) noexcept;

  explicit AABB(const AABB &second) noexcept;

  [[nodiscard]] auto getPoints(const ml::mat4 &transform, bool forceInvalidate = false) -> std::vector<Vector3f>;  // Called by collide(...)

  void               setMin(const Vector3f &min) noexcept;
  [[nodiscard]] auto getMin() const noexcept -> Vector3f;
  void               setMax(const Vector3f &max) noexcept;
  [[nodiscard]] auto getMax() const noexcept -> Vector3f;

  [[nodiscard]] bool operator==(const AABB &second) const noexcept;

  Vector3f getLocalPosition() override;

private:
  Vector3f              m_min{0.0f, 0.0f, 0.0f};
  Vector3f              m_max{0.0f, 0.0f, 0.0f};
  ml::mat4              m_oldTransform{};
  std::vector<Vector3f> m_pointsCache{};
};
