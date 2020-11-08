#pragma once

#include "Extension/Libraries/Vulkan.hpp"

#include <algorithm>
#include <vector>

namespace Vulkan {
  class SwapchainDetails;
}

class Vulkan::SwapchainDetails final {
public:
  VkSurfaceCapabilitiesKHR        m_capabilities{};
  std::vector<VkSurfaceFormatKHR> m_formats{};
  std::vector<VkPresentModeKHR>   m_presentModes{};

public:
  [[nodiscard]] auto getBestFormat() const -> const VkSurfaceFormatKHR &;
  [[nodiscard]] auto getBestPresentMode() const -> VkPresentModeKHR;
  [[nodiscard]] auto getSwapExtent(HWND window, std::uint32_t width, std::uint32_t height) const -> VkExtent2D;

  explicit SwapchainDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};
