#include "Transform.hpp"

Transform::Transform()
  : m_modelMatrix(Matrix4<float>{std::array<std::array<float, 4>, 4>{
  std::array<float, 4>{1.0f, 0.0f, 0.0f, 0.0f},
  std::array<float, 4>{0.0f, 1.0f, 0.0f, 0.0f},
  std::array<float, 4>{0.0f, 0.0f, 1.0f, 0.0f},
  std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f},
  }}) {}

Transform::Transform(Matrix4<float> &modelMatrix) : m_modelMatrix(modelMatrix) {}

bool Transform::operator==(const Transform &second) const noexcept {
  return m_modelMatrix == second.m_modelMatrix;
}