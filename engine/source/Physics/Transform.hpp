#pragma once

#include "Maths/Matrix.hpp"

class Transform {
public:
  Matrix4<float> m_modelMatrix{std::array<std::array<float, 4>, 4>{
  std::array<float, 4>{1.0f, 0.0f, 0.0f, 0.0f},
  std::array<float, 4>{0.0f, 1.0f, 0.0f, 0.0f},
  std::array<float, 4>{0.0f, 0.0f, 1.0f, 0.0f},
  std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f},
  }};

public:
  explicit Transform();
  explicit Transform(Matrix4<float> &modelMatrix);

  bool operator==(const Transform &transform) const noexcept;
};
