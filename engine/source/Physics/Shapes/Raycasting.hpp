#pragma once

#include "Maths/Vectors.hpp"

struct RayCollision {
  ICollisionShape* node;// Node that was hit
  Vector3f collidedAt;// WORLD SPACE pos of the collision !

  RayCollision(ICollisionShape *node, Vector3f collidedAt) : collidedAt(collidedAt), node(node) {}

  RayCollision() = default;
};

class Ray {
public :
  Ray(Vector3f position, Vector3f direction);
  ~Ray(void);

  Vector3f GetPosition() const {return position;}
  Vector3f GetDirection() const {return direction;}

protected :
  Vector3f position;// World space position
  Vector3f direction;// Normalised world space direction
};