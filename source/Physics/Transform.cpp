#include "Transform.hpp"

Transform::Transform() :
  m_modelMatrix(Matrix4<float> {})
{}

Transform::Transform(Matrix4<float> &modelMatrix) :
  m_modelMatrix(modelMatrix) {
}

bool Transform::operator==(const Transform &second) const noexcept {
  return m_modelMatrix == second.m_modelMatrix;
}