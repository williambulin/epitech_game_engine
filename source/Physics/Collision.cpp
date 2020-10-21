#include <math.h> /* fabs */
#include <iostream>
#include <algorithm>
#include <cfloat>

#include "Collision.hpp"

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

bool Collision::collide(AABB &firstCollider, Transform modelMatrixFirstCollider, AABB &secondCollider, Transform modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  // apply transformation
  Vector3f minFirstCollider  = firstCollider.getMin();
  Vector3f maxFirstCollider  = firstCollider.getMax();
  Vector3f minSecondCollider = secondCollider.getMin();
  Vector3f maxSecondCollider = secondCollider.getMax();

  if (maxFirstCollider.x > minSecondCollider.x && minFirstCollider.x < maxSecondCollider.x && maxFirstCollider.y > minSecondCollider.y && minFirstCollider.y < maxSecondCollider.y && maxFirstCollider.z > minSecondCollider.z && minFirstCollider.z < maxSecondCollider.z) {
    static const Vector3f faces[6] = {
    Vector3f(-1, 0, 0),
    Vector3f(1, 0, 0),
    Vector3f(0, -1, 0),
    Vector3f(0, 1, 0),
    Vector3f(0, 0, -1),
    Vector3f(0, 0, 1),
    };

    float distances[6] = {
    (maxSecondCollider.x - minFirstCollider.x),  // distance of box ’b’ to ’left ’ of ’a ’.
    (maxFirstCollider.x - minSecondCollider.x),  // distance of box ’b’ to ’right ’ of ’a ’.
    (maxSecondCollider.y - minFirstCollider.y),  // distance of box ’b’ to ’bottom ’ of ’a ’.
    (maxFirstCollider.y - minSecondCollider.y),  // distance of box ’b’ to ’top ’ of ’a ’.
    (maxSecondCollider.z - minFirstCollider.z),  // distance of box ’b’ to ’far ’ of ’a ’.
    (maxFirstCollider.z - minSecondCollider.z)   // distance of box ’b’ to ’near ’ of ’a ’.
    };
    float    penetration = FLT_MAX;
    Vector3f bestAxis(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 6; i++) {
      if (distances[i] < penetration) {
        penetration = distances[i];
        bestAxis    = faces[i];
      }
    }
    collisionInfo.addContactPoint(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), bestAxis, penetration);
    return (true);
  }
  return (false);
}

bool Collision::collide(const Sphere &firstCollider, Transform modelMatrixFirstCollider, const Sphere &secondCollider, Transform modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  // apply transformation
  float    radii = firstCollider.getRadius() + secondCollider.getRadius();
  Vector3f delta = secondCollider.getCenter() - firstCollider.getCenter();

  float deltaLength = delta.length();
  if (deltaLength < radii) {
    float penetration = (radii - deltaLength);
    delta.normalize();
    Vector3f normal = delta;
    Vector3f localA = normal * firstCollider.getRadius();
    Vector3f localB = (normal * -1) * secondCollider.getRadius();
    collisionInfo.addContactPoint(localA, localB, normal, penetration);
    return (true);
  }
  return (false);
}

bool Collision::collide(AABB &firstCollider, Transform modelMatrixFirstCollider, const Sphere &secondCollider, Transform modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  // apply transform
  Vector3f boxHalfSize       = (firstCollider.getMax() - firstCollider.getMin()) * 0.5f;
  Vector3f delta             = secondCollider.getCenter() - (firstCollider.getMin() + firstCollider.getMax()) * 0.5f;
  Vector3f closestPointOnBox = delta.clamp((boxHalfSize * -1), boxHalfSize);
  Vector3f localPoint        = delta - closestPointOnBox;
  float    distance          = localPoint.length();
  if (distance < secondCollider.getRadius()) {  // yes , we ’re colliding !
    localPoint.normalize();
    Vector3f collisionNormal = localPoint;
    float    penetration     = (secondCollider.getRadius() - distance);
    // empty
    Vector3f localA = Vector3f(0.0f, 0.0f, 0.0f);
    Vector3f localB = (collisionNormal * -1) * secondCollider.getRadius();
    collisionInfo.addContactPoint(localA, localB, collisionNormal, penetration);
    return (true);
  }
  return (false);
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
