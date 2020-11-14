#include "OBB.hpp"

OBB::OBB(const ml::vec3 &min, const ml::vec3 &max) noexcept : ICollisionShape{ShapeType::OBB}, m_min{min}, m_max{max} {}

OBB::OBB(const OBB &second) noexcept : ICollisionShape{ShapeType::OBB}, m_min{second.m_min}, m_max{second.m_max}, m_oldTransform{second.m_oldTransform}, m_pointsCache{second.m_pointsCache} {}

auto OBB::getPoints(const ml::mat4 &transform, bool forceInvalidate) -> std::vector<ml::vec3> {
  if (!forceInvalidate && transform == m_oldTransform && m_pointsCache.size() > 0)
    return m_pointsCache;

  std::vector<ml::vec3> points{
  m_min,
  ml::vec3{m_max.x, m_min.y, m_min.z},
  ml::vec3{m_max.x, m_max.y, m_min.z},
  ml::vec3{m_min.x, m_max.y, m_min.z},
  ml::vec3{m_min.x, m_max.y, m_max.z},
  ml::vec3{m_min.x, m_min.y, m_max.z},
  ml::vec3{m_max.x, m_min.y, m_max.z},
  m_max,
  };

  for (auto &point : points) {
    point = transform * point;
  }

  m_pointsCache  = points;
  m_oldTransform = transform;
  return points;
}  // Called by collide(...)

void OBB::setMin(const ml::vec3 &min) noexcept {
  m_min         = min;
  m_pointsCache = getPoints(m_oldTransform, true);
}

auto OBB::getMin() const noexcept -> ml::vec3 {
  return m_min;
}

void OBB::setMax(const ml::vec3 &max) noexcept {
  m_max         = max;
  m_pointsCache = getPoints(m_oldTransform, true);
}

auto OBB::getMax() const noexcept -> ml::vec3 {
  return m_max;
}

bool OBB::operator==(const OBB &second) const noexcept {
  return (second.m_min == m_min && second.m_max == m_max && second.m_oldTransform == m_oldTransform && second.m_pointsCache == second.m_pointsCache);
}

ml::vec3 OBB::getLocalPosition() const {
  return (m_max + m_min) * 0.5f;
}
