#pragma once

#include "Maths/Vectors.hpp"
#include "Physics/ICollisionShape.hpp"
#include "Components/Transform.hpp"

class Capsule final : public ICollisionShape {
public:
  explicit Capsule(const ml::vec3 &start, const ml::vec3 &end, const float &radius) noexcept;

  explicit Capsule(const Capsule &second) noexcept;

  [[nodiscard]] auto getPoints(const ml::mat4 &transform, bool forceInvalidate = false) -> std::vector<ml::vec3>;  // Called by collide(...)

  void               setStart(const ml::vec3 &start) noexcept;
  [[nodiscard]] auto getStart() const noexcept -> ml::vec3;
  void               setEnd(const ml::vec3 &end) noexcept;
  [[nodiscard]] auto getEnd() const noexcept -> ml::vec3;
  void               setRadius(const float &radius) noexcept;
  [[nodiscard]] auto getRadius() const noexcept -> float;

  [[nodiscard]] bool operator==(const Capsule &second) const noexcept;

  ml::vec3 getLocalPosition() override;

private:
  ml::vec3              m_start{0.0f, 0.0f, 0.0f};
  ml::vec3              m_end{0.0f, 0.0f, 0.0f};
  float                 m_radius{0.0f};
  ml::mat4              m_oldTransform{};
  std::vector<ml::vec3> m_pointsCache{};
};