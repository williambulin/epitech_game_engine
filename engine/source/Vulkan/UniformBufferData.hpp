#pragma once

#include "Extension/Libraries/glm.hpp"
#include "Maths/Math.hpp"
#include "Components/Light.hpp"

namespace Vulkan {
  struct UniformBufferData;
}

struct Vulkan::UniformBufferData {
  alignas(16) ml::mat4 model;
  alignas(16) ml::mat4 view;
  alignas(16) ml::mat4 projection;
  alignas(4) float lightSourceCount{0.0f};
  alignas(16) Components::Light::GLSLStruct lightSource[64]{};
};
