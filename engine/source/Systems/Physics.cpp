#include "Physics.hpp"

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
  auto     firstPoints       = firstCollider.getPoints(modelMatrixFirstCollider, true);
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

bool isMinkowskiFace(ml::vec3 edgeAFaceANormal, ml::vec3 edgeAFaceBNormal, ml::vec3 edgeADirection, ml::vec3 edgeBFaceANormal, ml::vec3 edgeBFaceBNormal, ml::vec3 edgeBDirection) {
  ml::vec3 edgeACross = edgeAFaceBNormal.cross(edgeAFaceANormal);
  ml::vec3 edgeBCross = edgeBFaceBNormal.cross(edgeBFaceANormal);

  float edgeBFaceADirection = edgeBFaceANormal.dot(edgeACross);
  float edgeBFaceBDirection = edgeBFaceBNormal.dot(edgeACross);
  float edgeAFaceADirection = edgeAFaceANormal.dot(edgeBCross);
  float edgeAFaceBDirection = edgeAFaceBNormal.dot(edgeBCross);

  return ((edgeBFaceADirection * edgeBFaceBDirection < 0) && (edgeAFaceADirection * edgeAFaceBDirection < 0) && (edgeBFaceADirection * edgeAFaceBDirection > 0));
}

bool queryEdgeCollisions(OBB &reference, OBB &incident, CollisionInfo &results) {
  auto edgesA = reference.m_edges;
  auto edgesB = reference.m_edges;
  for (int i = 0; i < edgesA.size(); i++) {
    auto     referenceFaceA   = reference.m_faces[std::get<OBB::FACES>(edgesA[i])[0]];
    auto     referenceFaceB   = reference.m_faces[std::get<OBB::FACES>(edgesA[i])[1]];
    ml::vec3 edgeAFaceANormal = std::get<OBB::NORMAL>(referenceFaceA);
    ml::vec3 edgeAFaceBNormal = std::get<OBB::NORMAL>(referenceFaceB);
    for (int j = 0; j < edgesB.size(); j++) {
      auto     incidentFaceA    = incident.m_faces[std::get<OBB::FACES>(edgesB[j])[0]];
      ml::vec3 edgeBFaceANormal = std::get<OBB::NORMAL>(incidentFaceA);
      edgeBFaceANormal *= -1;
      auto     incidentFaceB    = incident.m_faces[std::get<OBB::FACES>(edgesB[j])[1]];
      ml::vec3 edgeBFaceBNormal = std::get<OBB::NORMAL>(incidentFaceB);
      edgeBFaceBNormal *= -1;
      // COULD BE WRONG : (B - A) * transform == (B * transform - A * transform)
      ml::vec3 edgeADirection = std::get<OBB::EDGES>(edgesA[i])[1] - std::get<OBB::EDGES>(edgesA[i])[0];  // edgesA.get(i).getTransformedDirection(reference.getWorldTransform()).normalize();
      edgeADirection.normalize();
      ml::vec3 edgeBDirection = std::get<OBB::EDGES>(edgesA[j])[1] - std::get<OBB::EDGES>(edgesA[j])[0];  // edgesB.get(j).getTransformedDirection(incident.getWorldTransform()).normalize();
      edgeBDirection.normalize();

      if (isMinkowskiFace(edgeAFaceANormal, edgeAFaceBNormal, edgeADirection, edgeBFaceANormal, edgeBFaceBNormal, edgeBDirection)) {
        ml::vec3 axis = edgeADirection.cross(edgeBDirection);
        if (axis.length() == 0) {
          continue;
        }
        axis.normalize();
        ml::vec3 transformedPointA = std::get<OBB::EDGES>(edgesA[i])[0];
        ml::vec3 transformedPointB = std::get<OBB::EDGES>(edgesB[i])[0];

        if (axis.dot(transformedPointA - ((reference.m_pointsCache[reference.m_pointsCache.size() - 1] + reference.m_pointsCache[0]) * 0.5f)) < 0) {
          axis *= -1;
        }

        float distance = axis.dot(transformedPointB - transformedPointA);
        if (distance > 0) {
          return false;
        }
        if(distance > results.point.penetration) {
        /*                         results.setPenetration(distance);
                                results.setType(collisionType.ordinal());
                                results.setEnterNormal(axis);
                                results.setEdgeA(i);
                                results.setEdgeB(j); */
        results.addContactPoint(ml::vec3(0.0f, 0.0f, 0.0f), ml::vec3(0.0f, 0.0f, 0.0f), axis, distance);
        }
      }
    }
  }
  return true;
}

bool queryFaceCollisions(OBB &reference, OBB &incident, CollisionInfo &results) {
  for (int i = 0; i < reference.m_faces.size(); i++) {
    ml::vec3 axis       = std::get<OBB::NORMAL>(reference.m_faces[i]);
    ml::vec3 planePoint = reference.getSupport(axis);
    float    distance   = axis.dot(planePoint);
    //            Plane plane = new Plane(axis, planePoint);

    ml::vec3 negatedNormal = axis * -1.0f;
    ml::vec3 support       = incident.getSupport(negatedNormal);

    distance = axis.dot(planePoint) - distance;
    if (distance > 0) {
      return false;
    }
    if(distance > results.point.penetration) {
      results.addContactPoint(ml::vec3(0.0f, 0.0f, 0.0f), ml::vec3(0.0f, 0.0f, 0.0f), axis, distance);
    }
    // results.setPenetration(distance);
    // results.setEnterNormal(axis);
    // results.setReferenceFace(i);
  }
  return true;
}

bool Systems::Physics::collide(OBB &firstCollider, const ml::mat4 &modelMatrixFirstCollider, OBB &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  std::cout << "obb check"<< std::endl;
  auto firstPoints  = firstCollider.getPoints(modelMatrixFirstCollider, true);
  auto secondPoints = secondCollider.getPoints(modelMatrixSecondCollider, true);
  if (!queryFaceCollisions(firstCollider, secondCollider, collisionInfo)) {
    return true;
  }
  if (!queryFaceCollisions(secondCollider, firstCollider, collisionInfo)) {
    return true;
  }
  if (!queryEdgeCollisions(secondCollider, firstCollider, collisionInfo)) {
    return true;
  }

 /* if (results.getType() == Type.FACE_OF_A.ordinal()) {
    getFaceContactPoints(island.getColliderA(), island.getColliderB(), results);
  } else if (results.getType() == Type.FACE_OF_B.ordinal()) {
    getFaceContactPoints(island.getColliderB(), island.getColliderA(), results);
  } else {
    getEdgeContactPoint(island.getColliderA(), island.getColliderB(), results);
  }

  ml::vec3 offset = new ml::vec3();
  island.getColliderB().getPosition().sub(island.getColliderA().getPosition(), offset);
  if (results.getEnterNormal().dot(offset) > 0) {
    results.getEnterNormal().negate();
  }

  results.setCollided();

  return results;
}
return true;
*/
  return false;
}

auto Systems::Physics::getEntityWorldPosition(ICollisionShape &shape, const ml::mat4 &matrix) const -> ml::vec3 {
  return shape.getLocalPosition() * matrix.getTranslation();
}

bool Systems::Physics::checkCollisionExists(CollisionInfo existedOne, CollisionInfo toCompare) {
  if (existedOne.firstCollider == toCompare.firstCollider && existedOne.secondCollider == toCompare.secondCollider)
    return true;
  return false;
}

auto Systems::Physics::closestPointOnLineSegment(ml::vec3 A, ml::vec3 B, ml::vec3 Point) -> ml::vec3 {
  ml::vec3 AB = B - A;
  float    t  = (Point - A).dot(AB) / AB.dot(AB);
  // maybe parenthesis mistake
  return A + (AB * std::min(std::max(t, 0.0f), 1.0f));
}

bool Systems::Physics::collide(Capsule &firstCollider, const ml::mat4 &modelMatrixFirstCollider, Capsule &secondCollider, const ml::mat4 &modelMatrixSecondCollider, CollisionInfo &collisionInfo) noexcept {
  std::vector<ml::vec3> pointsFirstCollider{firstCollider.getPoints(modelMatrixFirstCollider)};
  std::vector<ml::vec3> pointsSecondCollider{secondCollider.getPoints(modelMatrixSecondCollider)};
  // capsule A:
  ml::vec3 a_Normal = pointsFirstCollider.front() - pointsFirstCollider.back();
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

  // select point on capsule B line segment nearest to best potential endpoint on A capsule:
  ml::vec3 bestB = Systems::Physics::closestPointOnLineSegment(b_A, b_B, bestA);

  // now do the same for capsule A segment:
  bestA                       = Systems::Physics::closestPointOnLineSegment(a_A, a_B, bestB);
  ml::vec3 penetration_normal = bestA - bestB;
  float    len                = penetration_normal.length();
  penetration_normal /= len;  // normalize
  float penetration_depth = firstCollider.getRadius() + secondCollider.getRadius() - len;
  if (penetration_depth > 0) {
    ml::vec3 collisionNormal = penetration_normal;
    float    penetration     = penetration_depth;
    ml::vec3 localA          = ml::vec3(0.0f, 0.0f, 0.0f);
    ml::vec3 localB          = ml::vec3(0.0f, 0.0f, 0.0f);
    collisionInfo.addContactPoint(localA, localB, collisionNormal, penetration);
    return true;
  }
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
      } else if (physicsI.m_shape->m_shapeType == ShapeType::CAPSULE && physicsJ.m_shape->m_shapeType == ShapeType::CAPSULE) {
        if (collide(reinterpret_cast<Capsule &>(*physicsJ.m_shape), transformJ.matrix, reinterpret_cast<Capsule &>(*physicsI.m_shape), transformI.matrix, info) == true)
          m_collisions.push_back(info);
      } else if (physicsI.m_shape->m_shapeType == ShapeType::OBB && physicsJ.m_shape->m_shapeType == ShapeType::OBB) {
        if (collide(reinterpret_cast<OBB &>(*physicsJ.m_shape), transformJ.matrix, reinterpret_cast<OBB &>(*physicsI.m_shape), transformI.matrix, info) == true)
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
  transformA.matrix.setTranslation(transformA.matrix.getTranslation() - (p.point.normal * p.point.penetration * (physA.getInverseMass() / totalMass)));
  transformB.matrix.setTranslation(transformB.matrix.getTranslation() + (p.point.normal * p.point.penetration * (physB.getInverseMass() / totalMass)));

  ml::vec3 relativeA{p.point.localA - getEntityWorldPosition(*physA.m_shape.get(), transformA.matrix)};
  ml::vec3 relativeB{p.point.localB - getEntityWorldPosition(*physB.m_shape.get(), transformB.matrix)};
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
