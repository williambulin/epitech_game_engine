#pragma once

#include "Maths/Math.hpp"

namespace Components {
  class Transform;
}

class Components::Transform final {
public:
  ml::mat4 matrix{
  {
  {1.0f, 0.0f, 0.0f, 0.0f},
  {0.0f, 1.0f, 0.0f, 0.0f},
  {0.0f, 0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f, 0.0f, 1.0f},
  },
  };
};
