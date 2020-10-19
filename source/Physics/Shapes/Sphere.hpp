
#pragma once

#include "Maths/Vectors.hpp"
#include "Physics/ICollisionShape.hpp"
#include "Physics/Transform.hpp"

class Sphere final : public ICollisionShape<Sphere> {
public:
  explicit Sphere(Vector3f center, float radius) noexcept;

  explicit Sphere(const Sphere &second) noexcept;

  void setCenter(const Vector3f &center) noexcept;
  [[nodiscard]] auto getCenter() const noexcept -> Vector3f;
  void setRadius(const float &radius) noexcept;
  [[nodiscard]] float getRadius() const noexcept;

  [[nodiscard]] bool operator==(const Sphere &second) const noexcept final;
private:
  Vector3f m_center{0.0f, 0.0f, 0.0f};
  float m_radius{0.0f};
  Transform m_oldTransform{};
};