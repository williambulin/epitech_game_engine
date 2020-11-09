#pragma once

#include "Extension/Libraries/glm.hpp"

namespace Components {
  class Movement;
}

class Components::Movement final {
public:
  glm::vec3 velocity{0.0f};
};
