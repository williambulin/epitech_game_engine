#pragma once

#include <vector>
#include <memory>

#include "Physics/Collision.hpp"
#include "GameObject.hpp"
#include "Shapes/Raycasting.hpp"

class CollisionSystem {
public:
  explicit CollisionSystem();
  void addCollider(std::shared_ptr<GameObject> collider);
  void collisionDections();
  void collisionResolution();

  void ImpulseResolveCollision(CollisionInfo &p) const;
  void IntegrateVelocity(float dt);
  bool RayIntersection (const Ray &r, GameObject &object, RayCollision& collision);

private:
  std::vector<std::shared_ptr<GameObject>> m_colliders;
  std::vector<CollisionInfo> m_collisions;
};