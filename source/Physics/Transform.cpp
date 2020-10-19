#include "Transform.hpp"

bool Transform::operator==(const Transform &second) const noexcept {
    return m_modelMatrix == second.m_modelMatrix;
}