#include "QueueFamilies.hpp"

bool Vulkan::QueueFamilies::isSuitable() {
  return m_graphicsQueue.has_value() && m_presentQueue.has_value();
}

auto Vulkan::QueueFamilies::getQueueFamiliesCreateInfo() -> std::vector<VkDeviceQueueCreateInfo> {
  std::vector<VkDeviceQueueCreateInfo> queueFamilies{};
  std::set<std::uint32_t>              queuesToCreate{m_graphicsQueue.value(), m_presentQueue.value()};

  for (auto queueIndex : queuesToCreate)
    queueFamilies.emplace_back(VkDeviceQueueCreateInfo{
    .sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    .queueFamilyIndex = queueIndex,
    .queueCount       = 1,
    .pQueuePriorities = std::addressof(m_queuePriority),
    });

  return queueFamilies;
}

Vulkan::QueueFamilies::QueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
  std::uint32_t queueFamiliesPropertiesCount{0};
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, std::addressof(queueFamiliesPropertiesCount), nullptr);
  std::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamiliesPropertiesCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, std::addressof(queueFamiliesPropertiesCount), queueFamiliesProperties.data());

  for (auto it{queueFamiliesProperties.cbegin()}; it != queueFamiliesProperties.cend(); ++it) {
    const auto &queueFamilyProperties{*it};
    auto        index{std::distance(queueFamiliesProperties.cbegin(), it)};

    if (queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
      m_graphicsQueue = index;

    VkBool32 hasPresentSupport{false};
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, static_cast<std::uint32_t>(index), surface, std::addressof(hasPresentSupport));

    if (hasPresentSupport)
      m_presentQueue = index;

    if (isSuitable())
      break;
  }
}
