#include "Physics.hpp"

bool Systems::Physics::collide(AABB &firstCollider, const ml::mat4 &modelMatrixFirstCollider, AABB &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  auto     firstPoints       = firstCollider.getPoints(modelMatrixFirstCollider, true);
  auto     secondPoints      = secondCollider.getPoints(modelMatrixSecondCollider, true);
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
    return true;
  }
  return false;
}

bool Systems::Physics::collide(const Sphere &firstCollider, const ml::mat4 &modelMatrixFirstCollider, const Sphere &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  auto     firstCenter  = firstCollider.getPoints(modelMatrixFirstCollider);
  auto     secondCenter = secondCollider.getPoints(modelMatrixSecondCollider);
  float    radii        = firstCollider.getRadius() + secondCollider.getRadius();
  Vector3f delta        = secondCenter - firstCenter;

  float deltaLength = delta.length();
  if (deltaLength < radii) {
    float penetration = (radii - deltaLength);
    delta.normalize();
    Vector3f normal = delta;
    Vector3f localA = normal * firstCollider.getRadius();
    Vector3f localB = (normal * -1) * secondCollider.getRadius();
    collisionInfo.addContactPoint(localA, localB, normal, penetration);
    return true;
  }
  return false;
}

bool Systems::Physics::collide(AABB &firstCollider, const ml::mat4 &modelMatrixFirstCollider, const Sphere &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  auto     firstPoints       = firstCollider.getPoints(modelMatrixFirstCollider, true);
  auto     secondCenter      = secondCollider.getPoints(modelMatrixSecondCollider);
  Vector3f minFirstCollider  = firstPoints.front();
  Vector3f maxFirstCollider  = firstPoints.back();
  Vector3f boxHalfSize       = (maxFirstCollider - minFirstCollider) * 0.5f;
  Vector3f delta             = secondCenter - (minFirstCollider + maxFirstCollider) * 0.5f;
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
    return true;
  }
  return false;
}

// Returns right hand perpendicular vector
Vector3f getNormal(const Vector3f &v) {
  return Vector3f(-v.y, v.x, v.z);
}

Vector3f getPerpendicularAxis(const std::vector<Vector3f> &vertices, std::size_t index) {
  Vector3f tmp(vertices[index + 1]);
  tmp = tmp - vertices[index];
  tmp.normalize();
  return getNormal(tmp);
}

// axes for which we'll test stuff. Two for each box, because testing for parallel axes isn't needed
std::vector<Vector3f> getPerpendicularAxes(const std::vector<Vector3f> &vertices1, const std::vector<Vector3f> &vertices2) {
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

bool Systems::Physics::collide(OBB &firstCollider, const ml::mat4 &modelMatrixFirstCollider, OBB &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  std::vector<Vector3f> verticesFirstCollider{firstCollider.getPoints(modelMatrixFirstCollider)};
  std::vector<Vector3f> verticesSecondCollider{secondCollider.getPoints(modelMatrixSecondCollider)};
  std::vector<Vector3f> axes{getPerpendicularAxes(verticesFirstCollider, verticesSecondCollider)};
  return true;
}

auto Systems::Physics::getEntityWorldPosition(ICollisionShape &shape, const ml::mat4 &matrix) const -> ml::vec3 {
  return shape.getLocalPosition() + matrix.getTranslation();
}

bool Systems::Physics::checkCollisionExists(CollisionInfo existedOne, CollisionInfo toCompare) {
  if (existedOne.firstCollider == toCompare.firstCollider && existedOne.secondCollider == toCompare.secondCollider)
    return true;
  return false;
}

void Systems::Physics::collisionDections() {
  auto &entities{getItems()};
  for (auto i = entities.begin(); i != entities.end(); i++) {
    for (auto j = i + 1; j != entities.end(); j++) {
      auto &&[entityI, transformI, physicsI]{*i};
      auto &&[entityJ, transformJ, physicsJ]{*j};

      if (entityI == entityJ)
        continue;

      CollisionInfo info{
      .firstCollider  = entityI,
      .secondCollider = entityJ,
      };

      auto it = std::find_if(m_collisions.begin(), m_collisions.end(), [this, info](CollisionInfo toCompare) {
        return checkCollisionExists(info, toCompare);
      });

      if (it != m_collisions.end())
        continue;

      if (physicsI.m_shape->m_shapeType == ShapeType::AABB && physicsJ.m_shape->m_shapeType == ShapeType::AABB) {
        if (collide(reinterpret_cast<AABB &>(*physicsI.m_shape), transformI.matrix, reinterpret_cast<AABB &>(*physicsJ.m_shape), transformJ.matrix, info) == true)
          m_collisions.push_back(info);
      } else if (physicsI.m_shape->m_shapeType == ShapeType::SPHERE && physicsJ.m_shape->m_shapeType == ShapeType::SPHERE) {
        if (collide(reinterpret_cast<Sphere &>(*physicsI.m_shape), transformI.matrix, reinterpret_cast<Sphere &>(*physicsJ.m_shape), transformJ.matrix, info) == true)
          m_collisions.push_back(info);
      } else if (physicsI.m_shape->m_shapeType == ShapeType::AABB && physicsJ.m_shape->m_shapeType == ShapeType::SPHERE) {
        if (collide(reinterpret_cast<AABB &>(*physicsI.m_shape), transformI.matrix, reinterpret_cast<Sphere &>(*physicsJ.m_shape), transformJ.matrix, info) == true)
          m_collisions.push_back(info);
      } else if (physicsI.m_shape->m_shapeType == ShapeType::SPHERE && physicsJ.m_shape->m_shapeType == ShapeType::AABB) {
        if (collide(reinterpret_cast<AABB &>(*physicsJ.m_shape), transformJ.matrix, reinterpret_cast<Sphere &>(*physicsI.m_shape), transformI.matrix, info) == true)
          m_collisions.push_back(info);
      }
    }
  }
}

void Systems::Physics::collisionResolution() {
  for (auto i{m_collisions.begin()}; i != m_collisions.end();) {
    if (i->framesLeft == 5) {
      // i->firstCollider->onCollisionBegin(i->secondCollider);
      // i->secondCollider->onCollisionBegin(i->firstCollider);
      impulseResolveCollision(*i);
    }
    i->framesLeft = i->framesLeft - 1;
    if (i->framesLeft < 0) {
      // TODO:
      // i->firstCollider->onCollisionEnd(i->secondCollider);
      // i->secondCollider->onCollisionEnd(i->firstCollider);
      i = m_collisions.erase(i);
    } else
      ++i;
  }
}

void Systems::Physics::impulseResolveCollision(CollisionInfo &p) const {
  auto &&[physA, transformA]{m_admin.getComponents<Components::Physics, Components::Transform>(p.firstCollider)};
  auto &&[physB, transformB]{m_admin.getComponents<Components::Physics, Components::Transform>(p.firstCollider)};

  float totalMass = physA.getInverseMass() + physB.getInverseMass();

  // Separate them out using projection
  // transformA.m_modelMatrix.setTranslation(p.firstCollider->getWorldPosition() - (p.point.normal * p.point.penetration * (physA.getInverseMass() / totalMass)));
  // transformB.m_modelMatrix.setTranslation(p.secondCollider->getWorldPosition() - (p.point.normal * p.point.penetration * (physB.getInverseMass() / totalMass)));

  Vector3f relativeA{p.point.localA - getEntityWorldPosition(*physA.m_shape.get(), transformA.matrix)};
  Vector3f relativeB{p.point.localB - getEntityWorldPosition(*physB.m_shape.get(), transformB.matrix)};
  Vector3f angVelocityA{physA.getAngularVelocity().cross(relativeA)};
  Vector3f angVelocityB{physB.getAngularVelocity().cross(relativeB)};

  Vector3f fullVelocityA{physA.getLinearVelocity() + angVelocityA};
  Vector3f fullVelocityB{physB.getLinearVelocity() + angVelocityB};
  Vector3f contactVelocity{fullVelocityB - fullVelocityA};

  float impulseForce = contactVelocity.dot(p.point.normal);

  // now to work out the effect of inertia ....
  Vector3f inertiaA      = static_cast<Vector3f>(physA.getInertiaTensor() * relativeA.cross(p.point.normal)).cross(relativeA);
  Vector3f inertiaB      = static_cast<Vector3f>(physB.getInertiaTensor() * relativeA.cross(p.point.normal)).cross(relativeB);
  float    angularEffect = (inertiaA + inertiaB).dot(p.point.normal);

  float cRestitution = 0.66f;  // disperse some kinectic energy

  float j = (-(1.0f + cRestitution) * impulseForce) / (totalMass + angularEffect);

  Vector3f fullImpulse = p.point.normal * j;
  if (!physA.getIsRigid()) {
    physA.applyLinearImpulse(fullImpulse * -1);
    physA.applyAngularImpulse(relativeA.cross(fullImpulse * -1));
  }
  if (!physB.getIsRigid()) {
    physB.applyLinearImpulse(fullImpulse);
    physB.applyAngularImpulse(relativeB.cross(fullImpulse));
  }
}

void Systems::Physics::integrateVelocity(float dt) {
  float dampingFactor = 1.0f - 0.95f;
  float frameDamping  = powf(dampingFactor, dt);

  auto &entities{getItems()};
  for (auto &&[entity, transform, physics] : entities) {
    // Position Stuff
    Vector3f position{transform.matrix.getTranslation()};
    Vector3f linearVel{physics.getLinearVelocity()};
    position += linearVel * dt;

    transform.matrix.setTranslation(position);
    // Linear Damping
    linearVel = linearVel * frameDamping;
    physics.setLinearVelocity(linearVel);

    // first implem angular
    Quaternion orientation{Quaternion::fromMatrix(transform.matrix.getRotation())};
    Vector3f   angVel{physics.getAngularVelocity()};

    Vector3f tempVec{angVel * dt * 0.5f};
    orientation = orientation + (Quaternion(tempVec.x, tempVec.y, tempVec.z, 0.0f) * orientation);

    orientation.normalize();
    transform.matrix.setRotation(orientation.toMatrix3());
    // Damp the angular velocity too
    angVel = angVel * frameDamping;
    physics.setAngularVelocity(angVel);
  }
}
