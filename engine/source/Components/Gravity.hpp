#pragma once

#include "Maths/Math.hpp"

namespace Components {
  class Gravity;
}

class Components::Gravity final {
public:
    ml::vec3 direction{0.0f, -1.0f, 0.0f};
    bool hasGravity = true; 
};