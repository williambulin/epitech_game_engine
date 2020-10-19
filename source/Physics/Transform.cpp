#include "Transform.hpp"

Transform::Transform(Matrix4 &modelMatrix) {
    m_modelMatrix = m_modelMatrix;
}

bool Transform::operator==(const Transform &second) const noexcept {
    return m_modelMatrix == second.m_modelMatrix;
}