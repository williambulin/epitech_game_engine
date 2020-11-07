#include "OBB.hpp"

OBB::OBB(const Vector3f &min, const Vector3f &max) noexcept : ICollisionShape(ShapeType::OBB), m_min{min}, m_max{max} {}

OBB::OBB(const OBB &second) noexcept : ICollisionShape(ShapeType::OBB), m_min{second.m_min}, m_oldTransform{second.m_oldTransform}, m_max{second.m_max}, m_pointsCache{second.m_pointsCache} {}

auto OBB::getPoints(Transform transform, bool forceInvalidate) -> std::vector<Vector3f> {
  if (!forceInvalidate && transform == m_oldTransform && m_pointsCache.size() > 0) {
    return m_pointsCache;
  }
  std::vector<Vector3f> points{};
  points.push_back(m_min);
  points.push_back(Vector3f{m_max.x, m_min.y, m_min.z});
  points.push_back(Vector3f{m_max.x, m_max.y, m_min.z});
  points.push_back(Vector3f{m_min.x, m_max.y, m_min.z});
  points.push_back(Vector3f{m_min.x, m_max.y, m_max.z});
  points.push_back(Vector3f{m_min.x, m_min.y, m_max.z});
  points.push_back(Vector3f{m_max.x, m_min.y, m_max.z});
  points.push_back(m_max);

  for (auto &point : points) {
    point = transform.m_modelMatrix * point;
  }
 
  m_pointsCache  = points;
  m_oldTransform = transform;
  return points;
}  // Called by collide(...)

void OBB::setMin(const Vector3f &min) noexcept {
  m_min         = min;
  m_pointsCache = getPoints(m_oldTransform, true);
}

auto OBB::getMin() const noexcept -> Vector3f {
  return m_min;
}

void OBB::setMax(const Vector3f &max) noexcept {
  m_max         = max;
  m_pointsCache = getPoints(m_oldTransform, true);
}

auto OBB::getMax() const noexcept -> Vector3f {
  return m_max;
}

bool OBB::operator==(const OBB &second) const noexcept {
  return (second.m_min == m_min && second.m_max == m_max && second.m_oldTransform == m_oldTransform && second.m_pointsCache == second.m_pointsCache);
}

//TODO IMPLEMENT
Vector3f OBB::getLocalPosition() {
  return Vector3f {1, 1, 1};
}
