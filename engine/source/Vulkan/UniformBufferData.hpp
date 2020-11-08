#pragma once

#include "Extension/Libraries/glm.hpp"

namespace Vulkan {
  struct UniformBufferData;
}

struct Vulkan::UniformBufferData {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 projection;
};
