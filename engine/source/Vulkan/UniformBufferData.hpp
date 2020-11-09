#pragma once

#include "Extension/Libraries/glm.hpp"
#include "Maths/Math.hpp"

namespace Vulkan {
  struct UniformBufferData;
}

struct Vulkan::UniformBufferData {
  alignas(16) ml::mat4 model;
  alignas(16) ml::mat4 view;
  alignas(16) ml::mat4 projection;
};
