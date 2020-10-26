#include "CollisionSystem.hpp"
#include "Collision.hpp"

void CollisionSystem::addCollider(GameObject &collider) {
  m_colliders.push_back(collider);
}

void CollisionSystem::collisionDections() {
  for (auto i : m_colliders) {
    for (auto j : m_colliders) {
      CollisionInfo info;
      info.firstCollider  = std::make_shared<GameObject>(i);
      info.secondCollider = std::make_shared<GameObject>(j);
      if (i.collider->m_shapeType == ICollisionShape::ShapeType::AABB && j.collider->m_shapeType == ICollisionShape::ShapeType::AABB) {
        if (Collision::collide((AABB &)*i.collider, i.modelMatrix, (AABB &)*j.collider, j.modelMatrix, info) == true) {
          m_collisions.push_back(info);
        }
      } else if (i.collider->m_shapeType == ICollisionShape::ShapeType::SPHERE && j.collider->m_shapeType == ICollisionShape::ShapeType::SPHERE) {
        CollisionInfo info;
        if (Collision::collide((Sphere &)*i.collider, i.modelMatrix, (Sphere &)*j.collider, j.modelMatrix, info) == true) {
          m_collisions.push_back(info);
        }
      } else if (i.collider->m_shapeType == ICollisionShape::ShapeType::AABB && j.collider->m_shapeType == ICollisionShape::ShapeType::SPHERE) {
        CollisionInfo info;
        if (Collision::collide((AABB &)*i.collider, i.modelMatrix, (Sphere &)*j.collider, j.modelMatrix, info) == true) {
          m_collisions.push_back(info);
        }
      } else if (i.collider->m_shapeType == ICollisionShape::ShapeType::SPHERE && j.collider->m_shapeType == ICollisionShape::ShapeType::AABB) {
        CollisionInfo info;
        if (Collision::collide((AABB &)*j.collider, j.modelMatrix, (Sphere &)*i.collider, i.modelMatrix, info) == true) {
          m_collisions.push_back(info);
        }
      }
    }
  }
}

void CollisionSystem::collisionResolution() {
  for (std::vector<CollisionInfo>::iterator i = m_collisions.begin(); i != m_collisions.end();) {
    if ((*i).framesLeft == 5) {
      i->firstCollider->onCollisionBegin(i->secondCollider);
      i->secondCollider->onCollisionBegin(i->firstCollider);
    }
    (*i).framesLeft = (*i).framesLeft - 1;
    if ((*i).framesLeft < 0) {
      i->firstCollider->onCollisionEnd(i->secondCollider);
      i->secondCollider->onCollisionEnd(i->firstCollider);
      i = m_collisions.erase(i);
    } else {
      ++i;
    }
  }
}