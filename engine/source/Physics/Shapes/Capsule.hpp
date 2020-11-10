#pragma once

#include "Maths/Vectors.hpp"
#include "Physics/ICollisionShape.hpp"
#include "Components/Transform.hpp"

class Capsule final : public ICollisionShape {
public:
  explicit Capsule(const Vector3f &start, const Vector3f &end, const float &radius) noexcept;

  explicit Capsule(const Capsule &second) noexcept;

  [[nodiscard]] auto getPoints(const ml::mat4 &transform, bool forceInvalidate = false) -> std::vector<Vector3f>;  // Called by collide(...)

  void               setStart(const Vector3f &start) noexcept;
  [[nodiscard]] auto getStart() const noexcept -> Vector3f;
  void               setEnd(const Vector3f &end) noexcept;
  [[nodiscard]] auto getEnd() const noexcept -> Vector3f;
  void               setRadius(const float &radius) noexcept;
  [[nodiscard]] auto getRadius() const noexcept -> float;

  [[nodiscard]] bool operator==(const Capsule &second) const noexcept;

  Vector3f getLocalPosition() override;

private:
  Vector3f              m_start{0.0f, 0.0f, 0.0f};
  Vector3f              m_end{0.0f, 0.0f, 0.0f};
  float                 m_radius{0.0f};
  ml::mat4              m_oldTransform{};
  std::vector<Vector3f> m_pointsCache{};
};