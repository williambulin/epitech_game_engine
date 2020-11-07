#include "Sphere.hpp"

Sphere::Sphere(Vector3f center, float radius) noexcept : ICollisionShape(ShapeType::SPHERE), m_center{center}, m_radius{radius} {}

Sphere::Sphere(const Sphere &second) noexcept : ICollisionShape(ShapeType::SPHERE), m_center{second.m_center}, m_radius{second.m_radius} {}

void Sphere::setCenter(const Vector3f &center) noexcept {
  m_center = center;
}
[[nodiscard]] auto Sphere::getCenter() const noexcept -> Vector3f {
  return m_center;
}

auto Sphere::getPoints(Transform transform) const noexcept -> Vector3f {
  return (transform.m_modelMatrix * m_center);
}

void Sphere::setRadius(const float &radius) noexcept {
  m_radius = radius;
}

[[nodiscard]] float Sphere::getRadius() const noexcept {
  return m_radius;
}

bool Sphere::operator==(const Sphere &second) const noexcept {
  return (m_center == second.m_center && m_radius == second.m_radius);
}

//TODO IMPLEMENT
Vector3f Sphere::getLocalPosition() {
  return Vector3f {1, 1, 1};
}