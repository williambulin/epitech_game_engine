#pragma once

#include <memory>
#include "Maths/Vectors.hpp"

class ICollisionShape {
public:
  virtual ~ICollisionShape()                             = default;
  enum ShapeType {
    UNKNOWN,
    AABB,
    SPHERE
  };

  virtual Vector3f getLocalPosition() = 0;

  ShapeType m_shapeType = UNKNOWN;
};