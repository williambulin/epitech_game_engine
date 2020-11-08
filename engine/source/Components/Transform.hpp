#pragma once

#include "Extension/Libraries/glm.hpp"

namespace Components {
  class Transform;
}

class Components::Transform final {
public:
  glm::vec3 position{0.0f};
  glm::vec3 rotation{0.0f};
  glm::vec3 scale{1.0f};

public:
  [[nodiscard]] inline auto toModelMatrix() const -> glm::mat4 {
    glm::mat4 mat{1.0f};
    mat = glm::scale(mat, scale);
    mat = glm::rotate(mat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    mat = glm::rotate(mat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    mat = glm::rotate(mat, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    mat = glm::translate(mat, position);
    return mat;
  }
};
