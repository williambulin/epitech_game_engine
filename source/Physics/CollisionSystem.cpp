#include <iostream>

#include "CollisionSystem.hpp"
#include "Collision.hpp"
#include "PhysicsObject.hpp"

CollisionSystem::CollisionSystem() {}

void CollisionSystem::addCollider(std::shared_ptr<GameObject> collider) {
  m_colliders.push_back(collider);
}

bool checkCollisionExists(CollisionInfo existedOne, CollisionInfo toCompare) {
  if (existedOne.firstCollider == toCompare.firstCollider && existedOne.secondCollider == toCompare.secondCollider) {
    return (true);
  }
  return (false);
}

void CollisionSystem::collisionDections() {
  for (auto i = m_colliders.begin(); i != m_colliders.end(); i++) {
    for (auto j = i + 1; j != m_colliders.end(); j++) {
      CollisionInfo info;
      info.firstCollider  = *i;
      info.secondCollider = *j;
      if (*i == *j)
        continue;
      auto it = std::find_if(m_collisions.begin(), m_collisions.end(), [info](CollisionInfo toCompare) {
        return checkCollisionExists(info, toCompare);
      });
      if (it != m_collisions.end()) {
        continue;
      }
      if ((*i)->m_collider->m_shapeType == ShapeType::AABB && (*j)->m_collider->m_shapeType == ShapeType::AABB) {
        if (Collision::collide((AABB &)*(*i)->m_collider, (*i)->m_modelMatrix, (AABB &)*(*j)->m_collider, (*j)->m_modelMatrix, info) == true) {
          m_collisions.push_back(info);
        }
      } else if ((*i)->m_collider->m_shapeType == ShapeType::SPHERE && (*j)->m_collider->m_shapeType == ShapeType::SPHERE) {
        if (Collision::collide((Sphere &)*(*i)->m_collider, (*i)->m_modelMatrix, (Sphere &)*(*j)->m_collider, (*j)->m_modelMatrix, info) == true) {
          m_collisions.push_back(info);
        }
      } else if ((*i)->m_collider->m_shapeType == ShapeType::AABB && (*j)->m_collider->m_shapeType == ShapeType::SPHERE) {
        if (Collision::collide((AABB &)*(*i)->m_collider, (*i)->m_modelMatrix, (Sphere &)*(*j)->m_collider, (*j)->m_modelMatrix, info) == true) {
          m_collisions.push_back(info);
        }
      } else if ((*i)->m_collider->m_shapeType == ShapeType::SPHERE && (*j)->m_collider->m_shapeType == ShapeType::AABB) {
        if (Collision::collide((AABB &)*(*j)->m_collider, (*j)->m_modelMatrix, (Sphere &)*(*i)->m_collider, (*i)->m_modelMatrix, info) == true) {
          m_collisions.push_back(info);
        }
      }
    }
  }
}

void CollisionSystem::collisionResolution() {
  for (std::vector<CollisionInfo>::iterator i = m_collisions.begin(); i != m_collisions.end();) {
    if (i->framesLeft == 5) {
      i->firstCollider->onCollisionBegin(i->secondCollider);
      i->secondCollider->onCollisionBegin(i->firstCollider);
      ImpulseResolveCollision((*i));
    }
    i->framesLeft = i->framesLeft - 1;
    if (i->framesLeft < 0) {
      i->firstCollider->onCollisionEnd(i->secondCollider);
      i->secondCollider->onCollisionEnd(i->firstCollider);
      i = m_collisions.erase(i);
    } else {
      ++i;
    }
  }
}

void CollisionSystem::ImpulseResolveCollision(CollisionInfo &p) const {
  PhysicsObject &physA = p.firstCollider->m_physicObject;   // TODO change this if not working
  PhysicsObject &physB = p.secondCollider->m_physicObject;  // TODO change this if not working

  Transform &transformA = p.firstCollider->m_modelMatrix;
  Transform &transformB = p.secondCollider->m_modelMatrix;

  float totalMass = physA.getInverseMass() + physB.getInverseMass();

  // Separate them out using projection
  //transformA.m_modelMatrix.setTranslation(p.firstCollider->getWorldPosition() - (p.point.normal * p.point.penetration * (physA.getInverseMass() / totalMass)));
  //transformB.m_modelMatrix.setTranslation(p.secondCollider->getWorldPosition() - (p.point.normal * p.point.penetration * (physB.getInverseMass() / totalMass)));

  Vector3f relativeA    = p.point.localA - p.firstCollider->getWorldPosition();
  Vector3f relativeB    = p.point.localB - p.secondCollider->getWorldPosition();
  Vector3f angVelocityA = physA.getAngularVelocity().cross(relativeA);
  Vector3f angVelocityB = physB.getAngularVelocity().cross(relativeB);

  Vector3f fullVelocityA   = physA.getLinearVelocity() + angVelocityA;
  Vector3f fullVelocityB   = physB.getLinearVelocity() + angVelocityB;
  Vector3f contactVelocity = fullVelocityB - fullVelocityA;

  float impulseForce = contactVelocity.dot(p.point.normal);

  // now to work out the effect of inertia ....
  Vector3f inertiaA      = static_cast<Vector3f>(physA.getInertiaTensor() * relativeA.cross(p.point.normal)).cross(relativeA);
  Vector3f inertiaB      = static_cast<Vector3f>(physB.getInertiaTensor() * relativeA.cross(p.point.normal)).cross(relativeB);
  float    angularEffect = (inertiaA + inertiaB).dot(p.point.normal);

  float cRestitution = 0.66f;  // disperse some kinectic energy

  float j = (-(1.0f + cRestitution) * impulseForce) / (totalMass + angularEffect);

  Vector3f fullImpulse = p.point.normal * j;
  Vector3f fullImpulseTemp = fullImpulse * -1;
  physA.applyLinearImpulse(fullImpulse * -1);
  physB.applyLinearImpulse(fullImpulse);

  physA.applyAngularImpulse(relativeA.cross(fullImpulse * -1));
  physB.applyAngularImpulse(relativeB.cross(fullImpulse));
}

void CollisionSystem::IntegrateVelocity(float dt) {
  float dampingFactor = 1.0f - 0.95f;
  float frameDamping  = powf(dampingFactor, dt);

  for (auto &object : m_colliders) {
    PhysicsObject &objectCollider = object->m_physicObject;
    if (object == nullptr) {
      continue;
    }
    Transform &transform = object->m_modelMatrix;
    // Position Stuff
    Vector3f position = transform.m_modelMatrix.getTranslation();
    Vector3f linearVel = objectCollider.getLinearVelocity();
    position += linearVel * dt;
    object->m_modelMatrix.m_modelMatrix.setTranslation(position);
    // Linear Damping
    linearVel = linearVel * frameDamping;
    objectCollider.setLinearVelocity(linearVel);
  }
}