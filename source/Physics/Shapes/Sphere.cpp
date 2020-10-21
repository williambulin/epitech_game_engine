#include "Sphere.hpp"

Sphere::Sphere(Vector3f center, float radius) noexcept : m_center{center}, m_radius{radius} {}

Sphere::Sphere(const Sphere &second) noexcept : m_center{second.m_center}, m_radius{second.m_radius} {}

void Sphere::setCenter(const Vector3f &center) noexcept {
  m_center = center;
}
[[nodiscard]] auto Sphere::getCenter() const noexcept -> Vector3f {
  return m_center;
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