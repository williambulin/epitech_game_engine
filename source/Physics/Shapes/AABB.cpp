#include <algorithm>

#include "AABB.hpp"

auto AABB::getPoints(Transform transform, bool forceInvalidate) -> std::vector<Vector3<float>> {
  if (!forceInvalidate && transform == m_oldTransform) {
    return m_pointsCache;
  }
  std::vector<Vector3<float>> points;
  points.push_back(m_min);
  points.push_back(Vector3<float> {m_max.x, m_min.y, m_min.z});
  points.push_back(Vector3<float> {m_max.x, m_max.y, m_min.z});
  points.push_back(Vector3<float> {m_min.x, m_max.y, m_min.z});
  points.push_back(Vector3<float> {m_min.x, m_max.y, m_max.z});
  points.push_back(Vector3<float> {m_min.x, m_min.y, m_max.z});
  points.push_back(Vector3<float> {m_max.x, m_min.y, m_max.z});
  points.push_back(m_max);
  //apply transform

  float max_x = (*std::max_element( points.begin(), points.end(),
              []( const Vector3<float> &a, const Vector3<float> &b ) {
                return a.x < b.x;
              })).x;
  float max_y = (*std::max_element( points.begin(), points.end(),
              []( const Vector3<float> &a, const Vector3<float> &b ) {
                return a.y < b.y;
              })).y;
  float max_z = (*std::max_element( points.begin(), points.end(),
              []( const Vector3<float> &a, const Vector3<float> &b ) {
                return a.z < b.z;
              })).z;
  float min_x = (*std::min_element( points.begin(), points.end(),
              []( const Vector3<float> &a, const Vector3<float> &b ) {
                return a.x < b.x;
              })).x;
  float min_y = (*std::min_element( points.begin(), points.end(),
              []( const Vector3<float> &a, const Vector3<float> &b ) {
                return a.y < b.y;
              })).y;
  float min_z = (*std::min_element( points.begin(), points.end(),
              []( const Vector3<float> &a, const Vector3<float> &b ) {
                return a.z < b.z;
              })).z;
  points.clear();
  points.push_back(Vector3<float> {min_x, min_y, min_z});
  points.push_back(Vector3<float> {max_x, min_y, min_z});
  points.push_back(Vector3<float> {max_x, max_y, min_z});
  points.push_back(Vector3<float> {min_x, max_y, min_z});
  points.push_back(Vector3<float> {min_x, max_y, max_z});
  points.push_back(Vector3<float> {min_x, min_y, max_z});
  points.push_back(Vector3<float> {max_x, min_y, max_z});
  points.push_back(Vector3<float> {max_x, max_y, max_z});
  m_pointsCache = points;
  m_oldTransform = transform;
  return points;
}

void AABB::setMin(const Vector3<float> &min) noexcept {
  m_min = min;
  m_pointsCache = getPoints(m_oldTransform, true);
}

void AABB::setMax(const Vector3<float> &max) noexcept {
  m_max = max;
  m_pointsCache = getPoints(m_oldTransform, true);
}

auto AABB::getMin() const noexcept -> Vector3<float> {
  return m_min;
}

auto AABB::getMax() const noexcept -> Vector3<float> {
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