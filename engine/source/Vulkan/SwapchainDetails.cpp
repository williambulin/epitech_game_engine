#include "SwapchainDetails.hpp"

auto Vulkan::SwapchainDetails::getBestFormat() const -> const VkSurfaceFormatKHR & {
  for (const auto &format : m_formats)
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
      return format;
  return m_formats.front();
}

auto Vulkan::SwapchainDetails::getBestPresentMode() const -> VkPresentModeKHR {
  for (auto presentMode : m_presentModes)
    if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)  // Triple Buffering
      return presentMode;
  return VK_PRESENT_MODE_FIFO_KHR;
}

auto Vulkan::SwapchainDetails::getSwapExtent(HWND window, std::uint32_t width, std::uint32_t height) const -> VkExtent2D {
  if (m_capabilities.currentExtent.width != UINT32_MAX)
    return m_capabilities.currentExtent;

  if (window != nullptr) {
    RECT rect{};
    GetWindowRect(window, std::addressof(rect));

    width  = static_cast<std::uint32_t>(rect.right - rect.left);
    height = static_cast<std::uint32_t>(rect.bottom - rect.top);
  }

  VkExtent2D extent{width, height};
  extent.width  = std::clamp(extent.width, m_capabilities.minImageExtent.width, m_capabilities.maxImageExtent.width);
  extent.height = std::clamp(extent.height, m_capabilities.minImageExtent.height, m_capabilities.maxImageExtent.height);
  return extent;
}

Vulkan::SwapchainDetails::SwapchainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, std::addressof(m_capabilities));

  std::uint32_t formatCount{0};
  vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, std::addressof(formatCount), nullptr);

  if (formatCount > 0) {
    m_formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, std::addressof(formatCount), m_formats.data());
  }

  std::uint32_t presentModesCount{0};
  vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, std::addressof(presentModesCount), nullptr);

  if (presentModesCount > 0) {
    m_presentModes.resize(presentModesCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, std::addressof(presentModesCount), m_presentModes.data());
  }
}
