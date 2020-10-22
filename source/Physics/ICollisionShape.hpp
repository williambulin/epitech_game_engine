#pragma once

#include <memory>

class ICollisionShape {
public:
  virtual ~ICollisionShape()                             = default;
  enum ShapeType {
    UNKNOWN,
    AABB,
    SPHERE
  };
  ShapeType m_shapeType = UNKNOWN;
};