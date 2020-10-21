#include <math.h>       /* fabs */
#include <iostream>
#include "Collision.hpp"

float Collision::SqDistPointAABB(const Vector3<float> &p, const Vector3<float> &aabbMin, const Vector3<float> &aabbMax) noexcept {
  float sqDist = 0.0f;
  if( p.x < aabbMin.x )
    sqDist += (aabbMin.x - p.x) * (aabbMin.x - p.x);
  if( p.x > aabbMax.x )
    sqDist += (p.x - aabbMax.x) * (p.x - aabbMax.x);
  if( p.y < aabbMin.y )
    sqDist += (aabbMin.y - p.y) * (aabbMin.y - p.y);
  if( p.y > aabbMax.y )
    sqDist += (p.y - aabbMax.y) * (p.y - aabbMax.y);
  if( p.z < aabbMin.z )
    sqDist += (aabbMin.z - p.z) * (aabbMin.z - p.z);
  if( p.z > aabbMax.z )
    sqDist += (p.z - aabbMax.z) * (p.z - aabbMax.z);
  return sqDist;
}

bool Collision::getSeparatingPlane(const Vector3<float> &rPos, const Vector3<float> &plane, const OBB &box1, const OBB &box2) noexcept {
  return (true);
/* return (fabs(rPos*plane) > 
        (fabs((box1.m_axisX*box1.m_halfSize.x)*plane) +
        fabs((box1.m_axisY*box1.m_halfSize.y)*plane) +
        fabs((box1.m_axisZ*box1.m_halfSize.z)*plane) +
        fabs((box2.m_axisX*box2.m_halfSize.x)*plane) + 
        fabs((box2.m_axisY*box2.m_halfSize.y)*plane) +
        fabs((box2.m_axisZ*box2.m_halfSize.z)*plane))); */
}

std::optional<std::vector<CollisionPointData>> Collision::collide(AABB &firstCollider, Transform modelMatrixFirstCollider, AABB &secondCollider, Transform modelMatrixSecondCollider) noexcept {
  std::cout << "In collide" << std::endl;
  std::vector<Vector3f> firstColliderHitbox = firstCollider.getPoints(modelMatrixFirstCollider, true);
  std::cout << "lul" << std::endl;
  for (Vector3f i: firstColliderHitbox) {
    std::cout << i.x << " | " << i[1] << " | " << i.z << std::endl;
  }
  std::vector<Vector3f> secondColliderHitbox = secondCollider.getPoints(modelMatrixSecondCollider, true);


  auto minFirstCollider = firstColliderHitbox.front();
  auto maxFirstCollider = firstColliderHitbox.back();
  auto minSecondCollider = secondColliderHitbox.front();
  auto maxSecondCollider = secondColliderHitbox.back();

  return (
    std::nullopt
/*     maxFirstCollider.x > minSecondCollider.x &&
    minFirstCollider.x < maxSecondCollider.x &&
    maxFirstCollider.y > minSecondCollider.y &&
    minFirstCollider.y < maxSecondCollider.y &&
    maxFirstCollider.z > minSecondCollider.z &&
    minFirstCollider.z < maxSecondCollider.z */
  );
}

bool Collision::collide(const Sphere &firstCollider, Transform modelMatrixFirstCollider, const Sphere &secondCollider, Transform modelMatrixSecondCollider) noexcept {
  auto firstColliderCenter = firstCollider.getCenter();
  auto firstColliderRadius = firstCollider.getRadius();
  auto secondColliderCenter = secondCollider.getCenter();
  auto secondColliderRadius = secondCollider.getRadius();
  //apply transformation
  float distance = (firstColliderCenter.x - secondColliderCenter.x) * (firstColliderCenter.x - secondColliderCenter.x) +
                   (firstColliderCenter.y - secondColliderCenter.y) * (firstColliderCenter.y - secondColliderCenter.y) +
                   (firstColliderCenter.z - secondColliderCenter.z) * (firstColliderCenter.z - secondColliderCenter.z);
  return distance < (firstColliderRadius + secondColliderRadius) * (firstColliderRadius + secondColliderRadius);
  return (true);
}

//https://gamedev.stackexchange.com/questions/156870/how-do-i-implement-a-aabb-sphere-collision
bool Collision::collide(const Sphere &firstCollider, Transform modelMatrixFirstCollider, AABB &secondCollider, Transform modelMatrixSecondCollider) noexcept {
  auto firstColliderCenter = firstCollider.getCenter();
  auto firstColliderRadius = firstCollider.getRadius();
  auto secondColliderHitbox = secondCollider.getPoints(modelMatrixSecondCollider);
  auto minSecondCollider = secondColliderHitbox.front();
  auto maxSecondCollider = secondColliderHitbox.back();
  //apply transformation
  float sqDist = Collision::SqDistPointAABB( firstColliderCenter, minSecondCollider, maxSecondCollider);
  return sqDist <= firstColliderRadius * firstColliderRadius;
  return (true);
}

bool Collision::collide(const OBB &firstCollider, const OBB &secondCollider) noexcept {
/*   Vec3 RPos = secondCollider.m_pos - firstCollider.m_pos;

  return !(Collision::getSeparatingPlane(RPos, firstCollider.m_axisX, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisY, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisZ, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, secondCollider.m_axisX, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, secondCollider.m_axisY, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, secondCollider.m_axisZ, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisX^secondCollider.m_axisX, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisX^secondCollider.m_axisY, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisX^secondCollider.m_axisZ, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisY^secondCollider.m_axisX, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisY^secondCollider.m_axisY, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisY^secondCollider.m_axisZ, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisZ^secondCollider.m_axisX, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisZ^secondCollider.m_axisY, firstCollider, secondCollider) ||
        Collision::getSeparatingPlane(RPos, firstCollider.m_axisZ^secondCollider.m_axisZ, firstCollider, secondCollider));
 */
  return (true);
}
