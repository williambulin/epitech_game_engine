#pragma once

#include "Maths/Matrix.hpp"

class Transform {
public:
  Matrix4<float> m_modelMatrix{};

public:
  explicit Transform(Matrix4 &modelMatrix);

  bool operator==(const Transform &transform) const noexcept;
};