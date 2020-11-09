#pragma once

#include "Extension/Libraries/Vulkan.hpp"

#include <optional>
#include <vector>
#include <set>

namespace Vulkan {
  class QueueFamilies;
}

class Vulkan::QueueFamilies final {
private:
  static constexpr float m_queuePriority{1.0f};

public:
  std::optional<std::uint32_t> m_graphicsQueue{0};
  std::optional<std::uint32_t> m_presentQueue{0};

public:
  [[nodiscard]] bool isSuitable();
  [[nodiscard]] auto getQueueFamiliesCreateInfo() -> std::vector<VkDeviceQueueCreateInfo>;

  explicit QueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};
