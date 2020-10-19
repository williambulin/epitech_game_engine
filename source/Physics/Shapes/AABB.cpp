#include "AABB.hpp"

auto AABB::getPoints(Transform transform, bool forceInvalidate = false) -> std::vector<Vec3> {
  if (!forceInvalidate && transform == m_oldTransform) {
    return m_pointsCache;
  }
  m_min * transform;
  m_max * transform;
}

void AABB::setMin(const Vec3 &min) noexcept {
  m_min = min;
  m_pointsCache = getPoints(m_oldTransform, true);
}

void AABB::setMax(const Vec3 &max) noexcept {
  m_max = max;
  m_pointsCache = getPoints(m_oldTransform, true);
}

auto AABB::getMin() const noexcept -> Vec3 {
  return m_min;
}

auto AABB::getMax() const noexcept -> Vec3 {
  return m_max;
}

bool AABB::operator==(const AABB &second) const noexcept {
  return (
    second.m_min == m_min &&
    second.m_max == m_max &&
    second.m_oldTransform == m_oldTransform &&
    second.m_pointsCache == second.m_pointsCache
  );
}