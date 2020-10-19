#include <math.h>       /* fabs */

#include "Collision.hpp"
#include "Transform.hpp"

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

bool Collision::getSeparatingPlane(const Vec3 &rPos, const Vec3 &plane, const OBB &box1, const OBB &box2) noexcept {
return (fabs(rPos*plane) > 
        (fabs((box1.m_axisX*box1.m_halfSize.x)*plane) +
        fabs((box1.m_axisY*box1.m_halfSize.y)*plane) +
        fabs((box1.m_axisZ*box1.m_halfSize.z)*plane) +
        fabs((box2.m_axisX*box2.m_halfSize.x)*plane) + 
        fabs((box2.m_axisY*box2.m_halfSize.y)*plane) +
        fabs((box2.m_axisZ*box2.m_halfSize.z)*plane)));
}

bool Collision::collide(const AABB &firstCollider, /*Mat4 infosCollier1,*/ const AABB &secondCollider /*, Mat4 infosCollier2*/) noexcept {
  return(firstCollider.m_topRight.x > secondCollider.m_bottomLeft.x &&
    firstCollider.m_bottomLeft.x < secondCollider.m_topRight.x &&
    firstCollider.m_topRight.y > secondCollider.m_bottomLeft.y &&
    firstCollider.m_bottomLeft.y < secondCollider.m_topRight.y &&
    firstCollider.m_topRight.z > secondCollider.m_bottomLeft.z &&
    firstCollider.m_bottomLeft.z < secondCollider.m_topRight.z);
}

bool Collision::collide(const Sphere &firstCollider, const Sphere &secondCollider) noexcept {
  float distance = (firstCollider.m_center.x - secondCollider.m_center.x) * (firstCollider.m_center.x - secondCollider.m_center.x) +
                   (firstCollider.m_center.y - secondCollider.m_center.y) * (firstCollider.m_center.y - secondCollider.m_center.y) +
                   (firstCollider.m_center.z - secondCollider.m_center.z) * (firstCollider.m_center.z - secondCollider.m_center.z);
  return distance < (firstCollider.radius + secondCollider.radius) * (firstCollider.radius + secondCollider.radius);
}

//https://gamedev.stackexchange.com/questions/156870/how-do-i-implement-a-aabb-sphere-collision
bool Collision::collide(const Sphere &firstCollider, const AABB &secondCollider) noexcept {
  float sqDist = Collision::SqDistPointAABB( firstCollider.m_center, secondCollider );
  return sqDist <= firstCollider.radius * firstCollider.radius;
}

bool Collision::collide(const OBB &firstCollider, const OBB &secondCollider) noexcept {
  Vec3 RPos = secondCollider.m_pos - firstCollider.m_pos;

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

}
