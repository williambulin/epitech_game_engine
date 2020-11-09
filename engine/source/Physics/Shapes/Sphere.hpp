
#pragma once

#include "Maths/Vectors.hpp"
#include "Physics/ICollisionShape.hpp"
#include "Components/Transform.hpp"

class Sphere final : public ICollisionShape {
public:
  explicit Sphere(Vector3f center, float radius) noexcept;
  explicit Sphere(const Sphere &second) noexcept;

  void                setCenter(const Vector3f &center) noexcept;
  [[nodiscard]] auto  getCenter() const noexcept -> Vector3f;
  void                setRadius(const float &radius) noexcept;
  [[nodiscard]] float getRadius() const noexcept;
  [[nodiscard]] auto  getPoints(const ml::mat4 &transform) const noexcept -> Vector3f;  // Called by collide(...)

  [[nodiscard]] bool operator==(const Sphere &second) const noexcept;

  Vector3f getLocalPosition() override;

private:
  Vector3f m_center{0.0f, 0.0f, 0.0f};
  float    m_radius{0.0f};
  ml::mat4 m_oldTransform{};
};
