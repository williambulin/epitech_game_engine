#pragma once

#include "Extension/Libraries/Vulkan.hpp"
#include "Extension/Libraries/glm.hpp"
#include "Image.hpp"
#include "Model.hpp"

#include <memory>

namespace Vulkan {
  class Drawable;
}

class Vulkan::Drawable final {
public:
  std::unique_ptr<Image> m_image{nullptr};
  std::unique_ptr<Model> m_model{nullptr};
  glm::vec3              m_position{0.0f, 0.0f, 0.0f};

public:
  explicit Drawable(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, glm::vec3 position, const std::string &name);
};
