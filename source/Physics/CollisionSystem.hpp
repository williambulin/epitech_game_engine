#pragma once

#include <vector>
#include <memory>

#include "Physics/Collision.hpp"
#include "GameObject.hpp"

class CollisionSystem {
public:
  explicit CollisionSystem();
  void addCollider(std::shared_ptr<GameObject> collider);
  void collisionDections();
  void collisionResolution();

  void ImpulseResolveCollision(CollisionInfo &p) const;
private:
  std::vector<std::shared_ptr<GameObject>> m_colliders;
  std::vector<CollisionInfo> m_collisions;
};