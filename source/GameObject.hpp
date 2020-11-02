#pragma once

#include <memory>

#include "Physics/ICollisionShape.hpp"
#include "Physics/Shapes/AABB.hpp"
#include "Physics/Transform.hpp"
#include "PhysicsObject.hpp"

class GameObject {
public:
  virtual void onCollisionBegin(std::shared_ptr<GameObject> otherObject) {
		//std::cout << "OnCollisionBegin event occured!\n";
	}

	virtual void onCollisionEnd(std::shared_ptr<GameObject> otherObject) {
		//std::cout << "OnCollisionEnd event occured!\n";
	}

  Vector3f getWorldPosition() const {
    return m_collider->getLocalPosition() + m_modelMatrix.m_modelMatrix.getTranslation();
  }

  std::shared_ptr<ICollisionShape> m_collider;
  Transform m_modelMatrix {};
  PhysicsObject m_physicObject;
};