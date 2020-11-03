#include <math.h> /* fabs */
#include <iostream>
#include <algorithm>
#include <cfloat>

#include "Collision.hpp"

bool Collision::collide(AABB &firstCollider, Transform modelMatrixFirstCollider, AABB &secondCollider, Transform modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  std::cout << "--------------------" << std::endl;
  std::cout << "Matrix first collider : " << std::endl;
  std::cout << modelMatrixFirstCollider.m_modelMatrix[0][0] << " | " << modelMatrixFirstCollider.m_modelMatrix[0][1] << " | " << modelMatrixFirstCollider.m_modelMatrix[0][2] << " | " << modelMatrixFirstCollider.m_modelMatrix[0][3] << " | " << std::endl;
  std::cout << modelMatrixFirstCollider.m_modelMatrix[1][0] << " | " << modelMatrixFirstCollider.m_modelMatrix[1][1] << " | " << modelMatrixFirstCollider.m_modelMatrix[1][2] << " | " << modelMatrixFirstCollider.m_modelMatrix[1][3] << " | " << std::endl;
  std::cout << modelMatrixFirstCollider.m_modelMatrix[2][0] << " | " << modelMatrixFirstCollider.m_modelMatrix[2][1] << " | " << modelMatrixFirstCollider.m_modelMatrix[2][2] << " | " << modelMatrixFirstCollider.m_modelMatrix[2][3] << " | " << std::endl;
  std::cout << modelMatrixFirstCollider.m_modelMatrix[3][0] << " | " << modelMatrixFirstCollider.m_modelMatrix[3][1] << " | " << modelMatrixFirstCollider.m_modelMatrix[3][2] << " | " << modelMatrixFirstCollider.m_modelMatrix[3][3] << " | " << std::endl;
  std::cout << "Matrix second collider : " << std::endl;
  std::cout << modelMatrixSecondCollider.m_modelMatrix[0][0] << " | " << modelMatrixSecondCollider.m_modelMatrix[0][1] << " | " << modelMatrixSecondCollider.m_modelMatrix[0][2] << " | " << modelMatrixSecondCollider.m_modelMatrix[0][3] << " | " << std::endl;
  std::cout << modelMatrixSecondCollider.m_modelMatrix[1][0] << " | " << modelMatrixSecondCollider.m_modelMatrix[1][1] << " | " << modelMatrixSecondCollider.m_modelMatrix[1][2] << " | " << modelMatrixSecondCollider.m_modelMatrix[1][3] << " | " << std::endl;
  std::cout << modelMatrixSecondCollider.m_modelMatrix[2][0] << " | " << modelMatrixSecondCollider.m_modelMatrix[2][1] << " | " << modelMatrixSecondCollider.m_modelMatrix[2][2] << " | " << modelMatrixSecondCollider.m_modelMatrix[2][3] << " | " << std::endl;
  std::cout << modelMatrixSecondCollider.m_modelMatrix[3][0] << " | " << modelMatrixSecondCollider.m_modelMatrix[3][1] << " | " << modelMatrixSecondCollider.m_modelMatrix[3][2] << " | " << modelMatrixSecondCollider.m_modelMatrix[3][3] << " | " << std::endl;
  std::cout << "--------------------" << std::endl;
  auto firstPoints = firstCollider.getPoints(modelMatrixFirstCollider, true);
  auto secondPoints = secondCollider.getPoints(modelMatrixSecondCollider, true);
  Vector3f minFirstCollider  = firstPoints.front();
  Vector3f maxFirstCollider  = firstPoints.back();
  Vector3f minSecondCollider = secondPoints.front();
  Vector3f maxSecondCollider = secondPoints.back();

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
    std::cout << "test" << std::endl;
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

// Returns right hand perpendicular vector
Vector3f getNormal(const Vector3f& v)
{
	return Vector3f(-v.y, v.x, v.z);
}

Vector3f getPerpendicularAxis(const std::vector<Vector3f>& vertices, std::size_t index) { 
  Vector3f tmp(vertices[index + 1]);
  tmp = tmp - vertices[index];
  tmp.normalize();
	return getNormal(tmp);
}

// axes for which we'll test stuff. Two for each box, because testing for parallel axes isn't needed
std::vector<Vector3f> getPerpendicularAxes(const std::vector<Vector3f>& vertices1, const std::vector<Vector3f>& vertices2) {
	std::vector<Vector3f> axes;

	axes.push_back(getPerpendicularAxis(vertices1, 0));
  axes.push_back(getPerpendicularAxis(vertices1, 0));
  axes.push_back(getPerpendicularAxis(vertices1, 0));
  axes.push_back(getPerpendicularAxis(vertices1, 0));
  axes.push_back(getPerpendicularAxis(vertices1, 0));
  axes.push_back(getPerpendicularAxis(vertices1, 0));
  axes.push_back(getPerpendicularAxis(vertices1, 0));
  axes.push_back(getPerpendicularAxis(vertices1, 0));
  axes.push_back(getPerpendicularAxis(vertices1, 0));
	axes[1] = getPerpendicularAxis(vertices1, 1);

	axes[2] = getPerpendicularAxis(vertices2, 0);
	axes[3] = getPerpendicularAxis(vertices2, 1);
	return axes;
}


bool Collision::collide(OBB &firstCollider, Transform modelMatrixFirstCollider, OBB &secondCollider, Transform modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  std::vector<Vector3f> verticesFirstCollider = firstCollider.getPoints(modelMatrixFirstCollider);
  std::vector<Vector3f> verticesSecondCollider = secondCollider.getPoints(modelMatrixSecondCollider);
  std::vector<Vector3f> axes = getPerpendicularAxes(verticesFirstCollider, verticesSecondCollider);
  return (true);
}
