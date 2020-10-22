#pragma once

#include <memory>

#include "Physics/ICollisionShape.hpp"
#include "Physics/Shapes/AABB.hpp"
#include "Physics/Transform.hpp"

class GameObject {
public:
  virtual void onCollisionBegin(std::shared_ptr<GameObject> otherObject) {
		//std::cout << "OnCollisionBegin event occured!\n";
	}

	virtual void onCollisionEnd(std::shared_ptr<GameObject> otherObject) {
		//std::cout << "OnCollisionEnd event occured!\n";
	}

  std::shared_ptr<ICollisionShape> collider;
  Transform modelMatrix;
};