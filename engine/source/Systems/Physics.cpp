#include "Physics.hpp"

void CollisionInfo::addContactPoint(const ml::vec3 &localA, const ml::vec3 &localB, const ml::vec3 &normal, float p) {
  point.localA      = localA;
  point.localB      = localB;
  point.normal      = normal;
  point.penetration = p;
}

bool Systems::Physics::collide(AABB &firstCollider, const ml::mat4 &modelMatrixFirstCollider, AABB &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  auto     firstPoints       = firstCollider.getPoints(modelMatrixFirstCollider, true);
  auto     secondPoints      = secondCollider.getPoints(modelMatrixSecondCollider, true);
  ml::vec3 minFirstCollider  = firstPoints.front();
  ml::vec3 maxFirstCollider  = firstPoints.back();
  ml::vec3 minSecondCollider = secondPoints.front();
  ml::vec3 maxSecondCollider = secondPoints.back();

  if (maxFirstCollider.x > minSecondCollider.x && minFirstCollider.x < maxSecondCollider.x && maxFirstCollider.y > minSecondCollider.y && minFirstCollider.y < maxSecondCollider.y && maxFirstCollider.z > minSecondCollider.z && minFirstCollider.z < maxSecondCollider.z) {
    static const ml::vec3 faces[6] = {
    ml::vec3(-1, 0, 0),
    ml::vec3(1, 0, 0),
    ml::vec3(0, -1, 0),
    ml::vec3(0, 1, 0),
    ml::vec3(0, 0, -1),
    ml::vec3(0, 0, 1),
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
    ml::vec3 bestAxis(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 6; i++) {
      if (distances[i] < penetration) {
        penetration = distances[i];
        bestAxis    = faces[i];
      }
    }
    // std::cout << "Collide AABB/AABB with penetration = " << penetration << std::endl;
    collisionInfo.addContactPoint(ml::vec3(0.0f, 0.0f, 0.0f), ml::vec3(0.0f, 0.0f, 0.0f), bestAxis, penetration);
    return true;
  }
  return false;
}

bool Systems::Physics::collide(const Sphere &firstCollider, const ml::mat4 &modelMatrixFirstCollider, const Sphere &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  auto     firstCenter  = firstCollider.getPoints(modelMatrixFirstCollider);
  auto     secondCenter = secondCollider.getPoints(modelMatrixSecondCollider);
  float    radii        = firstCollider.getRadius() + secondCollider.getRadius();
  ml::vec3 delta        = secondCenter - firstCenter;

  float deltaLength = delta.length();
  if (deltaLength < radii) {
    float penetration = (radii - deltaLength);
    delta.normalize();
    ml::vec3 normal = delta;
    ml::vec3 localA = normal * firstCollider.getRadius();
    ml::vec3 localB = (normal * -1) * secondCollider.getRadius();
    collisionInfo.addContactPoint(localA, localB, normal, penetration);
    return true;
  }
  return false;
}

bool Systems::Physics::collide(AABB &firstCollider, const ml::mat4 &modelMatrixFirstCollider, const Sphere &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  auto     firstPoints       = firstCollider.getPoints(modelMatrixFirstCollider);
  auto     secondCenter      = secondCollider.getPoints(modelMatrixSecondCollider);
  ml::vec3 minFirstCollider  = firstPoints.front();
  ml::vec3 maxFirstCollider  = firstPoints.back();
  ml::vec3 boxHalfSize       = (maxFirstCollider - minFirstCollider) * 0.5f;
  ml::vec3 delta             = secondCenter - ((maxFirstCollider + minFirstCollider) * 0.5f);
  ml::vec3 closestPointOnBox = delta.clamp((boxHalfSize * -1), boxHalfSize);
  ml::vec3 localPoint        = delta - closestPointOnBox;
  float    distance          = localPoint.length();
  if (distance < secondCollider.getRadius()) {  // yes , we ’re colliding !
    localPoint.normalize();
    ml::vec3 collisionNormal = localPoint;
    float    penetration     = (secondCollider.getRadius() - distance);
    // std::cout << "Collide with Sphere/AABB penetration = " << penetration << std::endl;
    // std::cout << "Collide with Sphere/AABB normal = " << collisionNormal.x << " | " << collisionNormal.y << " | " << collisionNormal.z << std::endl;
    // empty
    ml::vec3 localA = ml::vec3(0.0f, 0.0f, 0.0f);
    ml::vec3 localB = (collisionNormal * -1) * secondCollider.getRadius();
    collisionInfo.addContactPoint(localA, localB, collisionNormal, penetration);
    return true;
  }
  return false;
}

// Returns right hand perpendicular vector
ml::vec3 getNormal(const ml::vec3 &v) {
  return ml::vec3(-v.y, v.x, v.z);
}

ml::vec3 getPerpendicularAxis(const std::vector<ml::vec3> &vertices, std::size_t index) {
  ml::vec3 tmp(vertices[index + 1]);
  tmp = tmp - vertices[index];
  tmp.normalize();
  return getNormal(tmp);
}

// axes for which we'll test stuff. Two for each box, because testing for parallel axes isn't needed
std::vector<ml::vec3> getPerpendicularAxes(const std::vector<ml::vec3> &vertices1, const std::vector<ml::vec3> &vertices2) {
  std::vector<ml::vec3> axes;
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
  std::vector<ml::vec3> verticesFirstCollider{firstCollider.getPoints(modelMatrixFirstCollider)};
  std::vector<ml::vec3> verticesSecondCollider{secondCollider.getPoints(modelMatrixSecondCollider)};
  std::vector<ml::vec3> axes{getPerpendicularAxes(verticesFirstCollider, verticesSecondCollider)};
  return true;
}

auto Systems::Physics::getEntityWorldPositionAABB(const ICollisionShape &shape, const ml::mat4 &matrix) -> ml::vec3 {
  return matrix.getTranslation() * shape.getLocalPosition();
}

auto Systems::Physics::getEntityWorldPosition(const ICollisionShape &shape, const ml::mat4 &matrix) -> ml::vec3 {
  return matrix * shape.getLocalPosition();
}

bool Systems::Physics::checkCollisionExists(CollisionInfo existedOne, CollisionInfo toCompare) {
  if (existedOne.firstCollider == toCompare.firstCollider && existedOne.secondCollider == toCompare.secondCollider)
    return true;
  return false;
}

auto Systems::Physics::closestPointOnLineSegment(ml::vec3 A, ml::vec3 B, ml::vec3 Point) -> ml::vec3 {
  ml::vec3 AB = B - A;
  float    t  = (Point - A).dot(AB) / AB.dot(AB);
  // std::cout << "test nan = " << AB.dot(AB) << std::endl;
  // maybe parenthesis mistake
  return A + (AB * std::min(std::max(t, 0.0f), 1.0f));
}

bool Systems::Physics::collide(Capsule &firstCollider, const ml::mat4 &modelMatrixFirstCollider, Capsule &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  std::vector<ml::vec3> pointsFirstCollider{firstCollider.getPoints(modelMatrixFirstCollider)};
  std::vector<ml::vec3> pointsSecondCollider{secondCollider.getPoints(modelMatrixSecondCollider)};
  ml::vec3              a_Normal = pointsFirstCollider.front() - pointsFirstCollider.back();
  a_Normal.normalize();
  ml::vec3 a_LineEndOffset = a_Normal * firstCollider.getRadius();
  ml::vec3 a_A             = pointsFirstCollider.back() + a_LineEndOffset;
  ml::vec3 a_B             = pointsFirstCollider.front() - a_LineEndOffset;

  // capsule B:
  ml::vec3 b_Normal = pointsSecondCollider.front() - pointsSecondCollider.back();
  b_Normal.normalize();
  ml::vec3 b_LineEndOffset = b_Normal * secondCollider.getRadius();
  ml::vec3 b_A             = pointsSecondCollider.back() + b_LineEndOffset;
  ml::vec3 b_B             = pointsSecondCollider.front() - b_LineEndOffset;

  // vectors between line endpoints:
  ml::vec3 v0 = b_A - a_A;
  ml::vec3 v1 = b_B - a_A;
  ml::vec3 v2 = b_A - a_B;
  ml::vec3 v3 = b_B - a_B;

  // squared distances:
  float d0 = v0.dot(v0);
  float d1 = v1.dot(v1);
  float d2 = v2.dot(v2);
  float d3 = v3.dot(v3);

  // select best potential endpoint on capsule A:
  ml::vec3 bestA{0.0f, 0.0f, 0.0f};
  if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1) {
    bestA = a_B;
  } else {
    bestA = a_A;
  }

  ml::vec3 bestB = Systems::Physics::closestPointOnLineSegment(b_A, b_B, bestA);

  bestA                       = Systems::Physics::closestPointOnLineSegment(a_A, a_B, bestB);
  ml::vec3 penetration_normal = bestA - bestB;
  float    len                = penetration_normal.length();
  penetration_normal.normalize();
  float penetration_depth = firstCollider.getRadius() + secondCollider.getRadius() - len;
  if (penetration_depth > 0) {
    ml::vec3 collisionNormal = penetration_normal * -1.0f;
    float    penetration     = penetration_depth;
    ml::vec3 localA          = ml::vec3(0.0f, 0.0f, 0.0f);
    ml::vec3 localB          = ml::vec3(0.0f, 0.0f, 0.0f);
    collisionInfo.addContactPoint(localA, localB, collisionNormal, penetration);
    return true;
  }
  return false;
}

bool Systems::Physics::collide(Capsule &firstCollider, const ml::mat4 &modelMatrixFirstCollider, const Sphere &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  std::vector<ml::vec3> pointsFirstCollider{firstCollider.getPoints(modelMatrixFirstCollider)};
  auto                  secondCenter{secondCollider.getPoints(modelMatrixSecondCollider)};

  ml::vec3 a_Normal = pointsFirstCollider.front() - pointsFirstCollider.back();
  a_Normal.normalize();
  ml::vec3 a_LineEndOffset = a_Normal * firstCollider.getRadius();
  ml::vec3 a_A             = pointsFirstCollider.back() + a_LineEndOffset;
  ml::vec3 a_B             = pointsFirstCollider.front() - a_LineEndOffset;
  ml::vec3 bestA           = Systems::Physics::closestPointOnLineSegment(a_A, a_B, secondCenter);

  const ml::mat4 matrix{
  {
  {1.0f, 0.0f, 0.0f, 0.0f},
  {0.0f, 1.0f, 0.0f, 0.0f},
  {0.0f, 0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 0.0f, 1.0f},
  },
  };
  return (collide(Sphere(bestA, firstCollider.getRadius()), matrix, Sphere(secondCenter, secondCollider.getRadius()), matrix, collisionInfo));
}

bool Systems::Physics::collide(AABB &secondCollider, const ml::mat4 &modelMatrixSecondCollider, Capsule &firstCollider, const ml::mat4 &modelMatrixFirstCollider, CollisionInfo &collisionInfo) noexcept {
  std::vector<ml::vec3> pointsFirstCollider{firstCollider.getPoints(modelMatrixFirstCollider)};
  auto                  secondPoints{secondCollider.getPoints(modelMatrixSecondCollider)};
  auto                  secondCenter = Systems::Physics::getEntityWorldPosition(secondCollider, modelMatrixSecondCollider);
  ml::vec3              a_Normal     = pointsFirstCollider.front() - pointsFirstCollider.back();
  a_Normal.normalize();
  ml::vec3 a_LineEndOffset = a_Normal * firstCollider.getRadius();
  ml::vec3 a_A             = pointsFirstCollider.back() + a_LineEndOffset;
  ml::vec3 a_B             = pointsFirstCollider.front() - a_LineEndOffset;
  // std::cout << " vec a_A = " << a_A.x << " | " << a_A.y << " | " << a_A.z << std::endl;
  // std::cout << " vec a_B = " << a_B.x << " | " << a_B.y << " | " << a_B.z << std::endl;
  ml::vec3 bestA = Systems::Physics::closestPointOnLineSegment(a_A, a_B, secondCenter);
  // std::cout << " bestA = " << bestA.x << " | " << bestA.y << " | " << bestA.z << std::endl;
  const ml::mat4 matrix{
  {
  {1.0f, 0.0f, 0.0f, 0.0f},
  {0.0f, 1.0f, 0.0f, 0.0f},
  {0.0f, 0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 0.0f, 1.0f},
  },
  };
  AABB aabb{AABB(secondPoints.front(), secondPoints.back())};
  return (collide(aabb, matrix, Sphere(bestA, firstCollider.getRadius()), matrix, collisionInfo));
}

void Systems::Physics::collisionDections() {
  auto &entities{getItems()};
  for (auto i = entities.begin(); i != entities.end(); i++) {
    for (auto j = i + 1; j != entities.end(); j++) {
      auto &&[entityI, physicsI, transformI]{*i};
      auto &&[entityJ, physicsJ, transformJ]{*j};

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
        if (collide(reinterpret_cast<AABB &>(*physicsJ.m_shape), transformJ.matrix, reinterpret_cast<Sphere &>(*physicsI.m_shape), transformI.matrix, info) == true) {
          // std::cout << "in here" << std::endl;
          info.firstCollider  = entityJ;
          info.secondCollider = entityI;
          m_collisions.push_back(info);
        }
      } else if (physicsI.m_shape->m_shapeType == ShapeType::CAPSULE && physicsJ.m_shape->m_shapeType == ShapeType::CAPSULE) {
        if (collide(reinterpret_cast<Capsule &>(*physicsI.m_shape), transformI.matrix, reinterpret_cast<Capsule &>(*physicsJ.m_shape), transformJ.matrix, info) == true)
          m_collisions.push_back(info);
      } else if (physicsI.m_shape->m_shapeType == ShapeType::CAPSULE && physicsJ.m_shape->m_shapeType == ShapeType::SPHERE) {
        if (collide(reinterpret_cast<Capsule &>(*physicsI.m_shape), transformI.matrix, reinterpret_cast<Sphere &>(*physicsJ.m_shape), transformJ.matrix, info) == true)
          m_collisions.push_back(info);
      } else if (physicsI.m_shape->m_shapeType == ShapeType::SPHERE && physicsJ.m_shape->m_shapeType == ShapeType::CAPSULE) {
        if (collide(reinterpret_cast<Capsule &>(*physicsJ.m_shape), transformJ.matrix, reinterpret_cast<Sphere &>(*physicsI.m_shape), transformI.matrix, info) == true) {
          info.firstCollider  = entityJ;
          info.secondCollider = entityI;
          m_collisions.push_back(info);
        }
      } else if (physicsI.m_shape->m_shapeType == ShapeType::CAPSULE && physicsJ.m_shape->m_shapeType == ShapeType::AABB) {
        if (collide(reinterpret_cast<AABB &>(*physicsJ.m_shape), transformJ.matrix, reinterpret_cast<Capsule &>(*physicsI.m_shape), transformI.matrix, info) == true) {
          info.firstCollider  = entityJ;
          info.secondCollider = entityI;
          m_collisions.push_back(info);
        }
      } else if (physicsI.m_shape->m_shapeType == ShapeType::AABB && physicsJ.m_shape->m_shapeType == ShapeType::CAPSULE) {
        if (collide(reinterpret_cast<AABB &>(*physicsI.m_shape), transformI.matrix, reinterpret_cast<Capsule &>(*physicsJ.m_shape), transformJ.matrix, info) == true)
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
  auto &&[physB, transformB]{m_admin.getComponents<Components::Physics, Components::Transform>(p.secondCollider)};

  float totalMass = physA.getInverseMass() + physB.getInverseMass();

  // Separate them out using projection
  if (!physA.getIsRigid()) {
    transformA.matrix.setTranslation(transformA.matrix.getTranslation() - (p.point.normal * p.point.penetration * (physA.getInverseMass() / totalMass)));
  }
  if (!physB.getIsRigid()) {
    transformB.matrix.setTranslation(transformB.matrix.getTranslation() + (p.point.normal * p.point.penetration * (physB.getInverseMass() / totalMass)));
  }

  ml::vec3 relativeA{p.point.localA - getEntityWorldPosition(*physA.m_shape.get(), transformA.matrix)};
  ml::vec3 relativeB{p.point.localB - getEntityWorldPosition(*physB.m_shape.get(), transformB.matrix)};
  if ((*physA.m_shape.get()).m_shapeType == ShapeType::AABB && (*physB.m_shape.get()).m_shapeType == ShapeType::AABB) {
    relativeA = p.point.localA - getEntityWorldPositionAABB(*physA.m_shape.get(), transformA.matrix);
    relativeB = p.point.localB - getEntityWorldPositionAABB(*physB.m_shape.get(), transformB.matrix);
  }
  ml::vec3 angVelocityA{physA.getAngularVelocity().cross(relativeA)};
  ml::vec3 angVelocityB{physB.getAngularVelocity().cross(relativeB)};

  ml::vec3 fullVelocityA{physA.getLinearVelocity() + angVelocityA};
  ml::vec3 fullVelocityB{physB.getLinearVelocity() + angVelocityB};
  ml::vec3 contactVelocity{fullVelocityB - fullVelocityA};

  float impulseForce = contactVelocity.dot(p.point.normal);

  // now to work out the effect of inertia ....
  ml::vec3 inertiaA      = static_cast<ml::vec3>(physA.getInertiaTensor() * relativeA.cross(p.point.normal)).cross(relativeA);
  ml::vec3 inertiaB      = static_cast<ml::vec3>(physB.getInertiaTensor() * relativeA.cross(p.point.normal)).cross(relativeB);
  float    angularEffect = (inertiaA + inertiaB).dot(p.point.normal);

  float cRestitution = 0.66f;  // disperse some kinectic energy

  float j = (-(1.0f + cRestitution) * impulseForce) / (totalMass + angularEffect);

  ml::vec3 fullImpulse = p.point.normal * j;
  if (!physA.getIsRigid()) {
    physA.applyLinearImpulse(fullImpulse * -1);
    if ((*physA.m_shape.get()).m_shapeType != ShapeType::CAPSULE) {
      physA.applyAngularImpulse(relativeA.cross(fullImpulse * -1));
    }
  }
  if (!physB.getIsRigid()) {
    physB.applyLinearImpulse(fullImpulse);
    if ((*physB.m_shape.get()).m_shapeType != ShapeType::CAPSULE) {
      physB.applyAngularImpulse(relativeB.cross(fullImpulse));
    }
  }
}

void Systems::Physics::integrateVelocity(float dt) {
  float dampingFactor = 1.0f - 0.95f;
  float frameDamping  = powf(dampingFactor, dt);

  auto &entities{getItems()};
  for (auto &&[entity, physics, transform] : entities) {
    ml::vec3 position{transform.matrix.getTranslation()};
    ml::vec3 linearVel{physics.getLinearVelocity()};
    position += linearVel * dt;

    transform.matrix.setTranslation(position);
    // Linear Damping
    linearVel = linearVel * frameDamping;
    physics.setLinearVelocity(linearVel);

    // first implem angular
    Quaternion orientation{Quaternion::fromMatrix(transform.matrix.getRotation())};
    ml::vec3   angVel{physics.getAngularVelocity()};

    ml::vec3 tempVec{angVel * dt * 0.5f};
    orientation = orientation + (Quaternion(tempVec.x, tempVec.y, tempVec.z, 0.0f) * orientation);

    orientation.normalize();
    transform.matrix.setRotation(orientation.toMatrix3());
    // Damp the angular velocity too
    angVel = angVel * frameDamping;
    physics.setAngularVelocity(angVel);
  }
}

void Systems::Physics::update(float dt, std::uint64_t) {
  collisionDections();
  collisionResolution();
  integrateVelocity(dt);
}

bool Systems::Physics::RayIntersection(const Ray &r, RayCollision &collision) {
  auto &entities{getItems()};
  for (auto &&[entity, physics, transform] : entities) {
    switch (physics.m_shape->m_shapeType) {
      case ShapeType::AABB:
        if (RayAABBIntersection(r, transform.matrix, reinterpret_cast<AABB &>(*physics.m_shape), collision)) {
          collision.node = entity;
        }
        break;
      case ShapeType::OBB:
        if (RayOBBIntersection(r, transform.matrix, reinterpret_cast<OBB &>(*physics.m_shape), collision)) {
          collision.node = entity;
        }
        break;
      case ShapeType::SPHERE:
        if (RaySphereIntersection(r, transform.matrix, reinterpret_cast<Sphere &>(*physics.m_shape), collision)) {
          collision.node = entity;
        }
        break;
      case ShapeType::CAPSULE:
        if (RayCapsuleIntersection(r, transform.matrix, reinterpret_cast<Capsule &>(*physics.m_shape), collision)) {
          collision.node = entity;
        }
        break;
    }
  }
  if (collision.rayDistance > 0.0f)
    return true;
  return false;
}

bool Systems::Physics::RaySphereIntersection(const Ray &r, const ml::mat4 &worldTransform, const Sphere &volume, RayCollision &collision) {
  ml::vec3 spherePos    = Systems::Physics::getEntityWorldPosition(volume, worldTransform);
  float    sphereRadius = volume.getRadius();
  // Get the direction between the ray origin and the sphere origin
  ml::vec3 dir = (spherePos - r.GetPosition());
  // Then project the sphere ’s origin onto our ray direction vector
  float sphereProj = dir.dot(r.GetDirection());

  if (sphereProj < 0.0f) {
    return false;  // point is behind the ray !
  }
  // Get closest point on ray line to sphere
  ml::vec3 point      = r.GetPosition() + (r.GetDirection() * sphereProj);
  float    sphereDist = (point - spherePos).length();
  if (sphereDist > sphereRadius) {
    return false;
  }
  float offset          = sqrt((sphereRadius * sphereRadius) - (sphereDist * sphereDist));

  if (sphereProj - (offset) > collision.rayDistance && collision.rayDistance > 0)
    return false;
  collision.rayDistance = sphereProj - (offset);
  collision.collidedAt  = r.GetPosition() + (r.GetDirection() * collision.rayDistance);
  return true;
}

bool Systems::Physics::RayBoxIntersection(const Ray &r, const ml::vec3 &boxPos, const ml::vec3 &boxSize, RayCollision &collision) {
  ml::vec3 boxMin = boxPos - boxSize;
  ml::vec3 boxMax = boxPos + boxSize;
  ml::vec3 rayPos = r.GetPosition();
  ml::vec3 rayDir = r.GetDirection();
  ml::vec3 tVals(-1, -1, -1);
  for (int i = 0; i < 3; ++i) {  // get best 3 intersections
    if (rayDir[i] > 0) {
      tVals[i] = (boxMin[i] - rayPos[i]) / rayDir[i];
    } else if (rayDir[i] < 0) {
      tVals[i] = (boxMax[i] - rayPos[i]) / rayDir[i];
    }
  }
  float bestT = tVals.getMaxElement();
  if (bestT < 0.0f) {
    return false;  // no backwards rays !
  }
  ml::vec3    intersection = rayPos + (rayDir * bestT);
  const float epsilon      = 0.0001f;  // an amount of leeway in our calcs
  for (int i = 0; i < 3; ++i) {
    if (intersection[i] + epsilon < boxMin[i] || intersection[i] - epsilon > boxMax[i]) {
      return false;  // best intersection doesn ’t touch the box !
    }
  }
  if (bestT > collision.rayDistance && collision.rayDistance > 0)
    return false;
  collision.collidedAt  = intersection;
  collision.rayDistance = bestT;
  return true;
}

bool Systems::Physics::RayAABBIntersection(const Ray &r, const ml::mat4 &worldTransform, AABB &volume, RayCollision &collision) {
  ml::vec3 boxPos           = Systems::Physics::getEntityWorldPosition(volume, worldTransform);
  auto     firstPoints      = volume.getPoints(worldTransform);
  auto     minFirstCollider = firstPoints.front();
  auto     maxFirstCollider = firstPoints.back();
  auto     boxSize          = (maxFirstCollider - minFirstCollider) * 0.5f;
  return RayBoxIntersection(r, boxPos, boxSize, collision);
}

bool Systems::Physics::RayOBBIntersection(const Ray &r, const ml::mat4 &worldTransform, OBB &volume, RayCollision &collision) {
  Quaternion orientation  = Quaternion::fromMatrix(worldTransform.getRotation());
  ml::vec3   position     = Systems::Physics::getEntityWorldPosition(volume, worldTransform);
  auto       transform    = orientation.toMatrix3();
  auto       invTransform = orientation.conjugate().toMatrix3();
  ml::vec3   localRayPos  = r.GetPosition() - position;
  Ray        tempRay(invTransform * localRayPos, invTransform * r.GetDirection());
  auto       firstPoints      = volume.getPoints(worldTransform);
  auto       minFirstCollider = firstPoints.front();
  auto       maxFirstCollider = firstPoints.back();
  auto       boxSize          = (maxFirstCollider - minFirstCollider) * 0.5f;

  bool collided = RayBoxIntersection(tempRay, ml::vec3(0, 0, 0), boxSize, collision);
  if (collided) {
    collision.collidedAt = transform * collision.collidedAt + position;
  }
  return collided;
}

bool Systems::Physics::RayCapsuleIntersection(const Ray &r, const ml::mat4 &worldTransform, Capsule &volume, RayCollision &collision) {
  std::vector<ml::vec3> pointsFirstCollider{volume.getPoints(worldTransform)};
  ml::vec3              a_Normal = pointsFirstCollider.front() - pointsFirstCollider.back();
  a_Normal.normalize();
  ml::vec3 a_LineEndOffset = a_Normal * volume.getRadius();
  ml::vec3 a_A             = pointsFirstCollider.back() + a_LineEndOffset;
  ml::vec3 a_B             = pointsFirstCollider.front() - a_LineEndOffset;

  // RAY
  ml::vec3 b_A = r.GetPosition();
  ml::vec3 b_B = r.GetPosition() + (r.GetDirection() * 100000);  // TODO update this to handle infinity

  // vectors between line endpoints:
  ml::vec3 v0 = b_A - a_A;
  ml::vec3 v1 = b_B - a_A;
  ml::vec3 v2 = b_A - a_B;
  ml::vec3 v3 = b_B - a_B;

  // squared distances:
  float d0 = v0.dot(v0);
  float d1 = v1.dot(v1);
  float d2 = v2.dot(v2);
  float d3 = v3.dot(v3);

  // select best potential endpoint on capsule A:
  ml::vec3 bestA{0.0f, 0.0f, 0.0f};
  if (d2 < d0 || d2 < d1 || d3 < d0 || d3 < d1) {
    bestA = a_B;
  } else {
    bestA = a_A;
  }

  ml::vec3 bestB = Systems::Physics::closestPointOnLineSegment(b_A, b_B, bestA);

  bestA                       = Systems::Physics::closestPointOnLineSegment(a_A, a_B, bestB);
  ml::vec3 penetration_normal = bestA - bestB;
  float    len                = penetration_normal.length();
  penetration_normal.normalize();
  float penetration_depth = volume.getRadius() - len;
  if (penetration_depth > 0 && (r.GetPosition() - r.GetPosition()).length() < collision.rayDistance) {
    collision.collidedAt  = Systems::Physics::getEntityWorldPosition(volume, worldTransform);
    collision.rayDistance = (r.GetPosition() - r.GetPosition()).length();
    return true;
  }
  return false;
}
