#pragma once

#include "Maths/Math.hpp"

namespace Components {
  class Camera;
}

class Components::Camera final {
public:
  ml::vec3 m_position{0.0f, 0.0f, 0.0f};
  ml::vec3 m_front{0.0f, 0.0f, -1.0f};
  ml::vec3 m_up{0.0f, 1.0f, 0.0f};
  ml::vec3 m_right{0.0f, 0.0f, 0.0f};
  ml::vec3 m_worldUp{0.0f, 1.0f, 0.0f};

public:
  float    fov{80.0f};
  ml::vec3 angles{0.0f, 0.0f, 0.0f};

  [[nodiscard]] inline auto viewMatrix() -> ml::mat4 {
    m_front.x = cos(ml::radians(angles.y)) * cos(ml::radians(angles.x));
    m_front.y = sin(ml::radians(angles.x));
    m_front.z = sin(ml::radians(angles.y)) * cos(ml::radians(angles.x));
    m_front.normalize();
    m_right = m_front.cross(m_worldUp);
    m_right.normalize();
    m_up = m_right.cross(m_front);
    m_up.normalize();
    return Matrix4<float>::lookAt(m_position, m_position + m_front, m_up);
  }

  [[nodiscard]] inline auto projectionMatrix(float aspectRatio) const -> ml::mat4 {
    return Matrix4<float>::perspective(ml::radians(fov), aspectRatio, 0.01f, 500.0f);
  }
};
