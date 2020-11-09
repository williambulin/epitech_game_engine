#include "Drawable.hpp"

Vulkan::Drawable::Drawable(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, glm::vec3 position, const std::string &name) : m_position{position} {
  m_image = std::make_unique<Image>(device, physicalDevice, commandPool, graphicsQueue, "../resources/" + name + ".png");
  m_model = std::make_unique<Model>(physicalDevice, device, commandPool, graphicsQueue, "../resources/" + name + ".obj");
}
