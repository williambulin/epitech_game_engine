#pragma once

#include "../ICollisionShape.hpp"
#include "Maths/Vectors.hpp"

struct RayCollision {
  ICollisionShape* node;// Node that was hit
  Vector3f collidedAt;// WORLD SPACE pos of the collision !
  float rayDistance;

  RayCollision(ICollisionShape *node, ml::vec3 collidedAt, float rayDistance = 0) : collidedAt(collidedAt), node(node), rayDistance(rayDistance) {}

  RayCollision() = default;
};

class Ray {
public :
  Ray(ml::vec3 position, ml::vec3 direction);
  ~Ray(void);

  ml::vec3 GetPosition() const {return position;}
  ml::vec3 GetDirection() const {return direction;}

protected :
  ml::vec3 position;// World space position
  ml::vec3 direction;// Normalised world space direction
};