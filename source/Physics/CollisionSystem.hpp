#pragma once

#include <vector>
#include <memory>

#include "Physics/Collision.hpp"
#include "GameObject.hpp"

class CollisionSystem {
public:
  void addCollider(GameObject &collider);
  void collisionDections();
  void collisionResolution();

  void ImpulseResolveCollision(CollisionInfo &p) const;
private:
  std::vector<GameObject> &m_colliders;
  std::vector<CollisionInfo> m_collisions;
};