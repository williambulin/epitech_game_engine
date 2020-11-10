#pragma once

#include <memory>
#include "Maths/Math.hpp"
#include "Maths/Vectors.hpp"
#include "Physics/ShapeType.hpp"

class ICollisionShape {
public:
  explicit ICollisionShape() noexcept : m_shapeType(ShapeType::UNKNOWN) {}
  explicit ICollisionShape(ShapeType t) noexcept : m_shapeType(t) {}
  virtual ~ICollisionShape() = default;

  virtual ml::vec3 getLocalPosition() = 0;

  ShapeType m_shapeType;
};
