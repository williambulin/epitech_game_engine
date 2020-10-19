#include <algorithm>

#include "AABB.hpp"

auto AABB::getPoints(Transform transform, bool forceInvalidate = false) -> std::vector<Vec3> {
  if (!forceInvalidate && transform == m_oldTransform) {
    return m_pointsCache;
  }
  std::vector<Vec3> points;
  points.push_back(m_min);
  points.push_back(Vec3 {m_max.x, m_min.y, m_min.z});
  points.push_back(Vec3 {m_max.x, m_max.y, m_min.z});
  points.push_back(Vec3 {m_min.x, m_max.y, m_min.z});
  points.push_back(Vec3 {m_min.x, m_max.y, m_max.z});
  points.push_back(Vec3 {m_min.x, m_min.y, m_max.z});
  points.push_back(Vec3 {m_max.x, m_min.y, m_max.z});
  points.push_back(m_max);
  //apply transform

  float max_x = (*std::max_element( points.begin(), points.end(),
              []( const Vec3 &a, const Vec3 &b ) {
                return a.x < b.x;
              })).x;
  float max_y = (*std::max_element( points.begin(), points.end(),
              []( const Vec3 &a, const Vec3 &b ) {
                return a.y < b.y;
              })).y;
  float max_z = (*std::max_element( points.begin(), points.end(),
              []( const Vec3 &a, const Vec3 &b ) {
                return a.z < b.z;
              })).z;
  float min_x = (*std::min_element( points.begin(), points.end(),
              []( const Vec3 &a, const Vec3 &b ) {
                return a.x < b.x;
              })).x;
  float min_y = (*std::min_element( points.begin(), points.end(),
              []( const Vec3 &a, const Vec3 &b ) {
                return a.y < b.y;
              })).y;
  float min_z = (*std::min_element( points.begin(), points.end(),
              []( const Vec3 &a, const Vec3 &b ) {
                return a.z < b.z;
              })).z;
  points.clear();
  points.push_back(Vec3 {min_x, min_y, min_z});
  points.push_back(Vec3 {max_x, min_y, min_z});
  points.push_back(Vec3 {max_x, max_y, min_z});
  points.push_back(Vec3 {min_x, max_y, min_z});
  points.push_back(Vec3 {min_x, max_y, max_z});
  points.push_back(Vec3 {min_x, min_y, max_z});
  points.push_back(Vec3 {max_x, min_y, max_z});
  points.push_back(Vec3 {max_x, max_y, max_z});
  return points;
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