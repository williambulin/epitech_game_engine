
#pragma once

#include "Extension/Language/Library.hpp"
#include "Maths/Vectors.hpp"
#include "Physics/ICollisionShape.hpp"
#include "Components/Transform.hpp"

class Sphere final : public ICollisionShape {
public:
  DLLATTRIB explicit Sphere(ml::vec3 center, float radius) noexcept;
  DLLATTRIB explicit Sphere(const Sphere &second) noexcept;

  void                setCenter(const ml::vec3 &center) noexcept;
  [[nodiscard]] auto  getCenter() const noexcept -> ml::vec3;
  void                setRadius(const float &radius) noexcept;
  [[nodiscard]] float getRadius() const noexcept;
  [[nodiscard]] auto  getPoints(const ml::mat4 &transform) const noexcept -> ml::vec3;  // Called by collide(...)

  [[nodiscard]] bool operator==(const Sphere &second) const noexcept;

  ml::vec3 getLocalPosition() override;

private:
  ml::vec3 m_center{0.0f, 0.0f, 0.0f};
  float    m_radius{0.0f};
  ml::mat4 m_oldTransform{};
};
