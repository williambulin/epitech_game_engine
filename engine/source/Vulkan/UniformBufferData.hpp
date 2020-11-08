#pragma once

#include "Extension/Libraries/glm.hpp"
#include "Maths/Matrix.hpp"

namespace Vulkan {
  struct UniformBufferData;
}

struct Vulkan::UniformBufferData {
  alignas(16) glm::mat4 model;
  alignas(16) Matrix4<float> view;
  alignas(16) Matrix4<float> projection;
};
