#include "Collision.hpp"

float Collision::SqDistPointAABB(const Vec3 &p, const AABB &b ) noexcept {
  float sqDist = 0.0f;
  if( p.x < b.m_bottomLeft.x )
    sqDist += (b.m_bottomLeft.x - p.x) * (b.m_bottomLeft.x - p.x);
  if( p.x > b.m_topRight.x )
    sqDist += (p.x - b.m_topRight.x) * (p.x - b.m_topRight.x);
  if( p.y < b.m_bottomLeft.y )
    sqDist += (b.m_bottomLeft.y - p.y) * (b.m_bottomLeft.y - p.y);
  if( p.y > b.m_topRight.y )
    sqDist += (p.y - b.m_topRight.y) * (p.y - b.m_topRight.y);
  if( p.z < b.m_bottomLeft.z )
    sqDist += (b.m_bottomLeft.z - p.z) * (b.m_bottomLeft.z - p.z);
  if( p.z > b.m_topRight.z )
    sqDist += (p.z - b.m_topRight.z) * (p.z - b.m_topRight.z);
  return sqDist;
}

bool Collision::collide(const AABB &first_collider, const AABB &second_collider) noexcept {
  return(first_collider.m_topRight.x > second_collider.m_bottomLeft.x &&
    first_collider.m_bottomLeft.x < second_collider.m_topRight.x &&
    first_collider.m_topRight.y > second_collider.m_bottomLeft.y &&
    first_collider.m_bottomLeft.y < second_collider.m_topRight.y &&
    first_collider.m_topRight.z > second_collider.m_bottomLeft.z &&
    first_collider.m_bottomLeft.z < second_collider.m_topRight.z);
}

bool Collision::collide(const Sphere &first_collider, const Sphere &second_collider) noexcept {
  float distance = (first_collider.m_center.x - second_collider.m_center.x) * (first_collider.m_center.x - second_collider.m_center.x) +
                   (first_collider.m_center.y - second_collider.m_center.y) * (first_collider.m_center.y - second_collider.m_center.y) +
                   (first_collider.m_center.z - second_collider.m_center.z) * (first_collider.m_center.z - second_collider.m_center.z);
  return distance < (first_collider.radius + second_collider.radius) * (first_collider.radius + second_collider.radius);
}

//https://gamedev.stackexchange.com/questions/156870/how-do-i-implement-a-aabb-sphere-collision
bool Collision::collide(const Sphere &first_collider, const AABB &second_collider) noexcept {
  float sqDist = Collision::SqDistPointAABB( first_collider.m_center, second_collider );
  return sqDist <= first_collider.radius * first_collider.radius;
}