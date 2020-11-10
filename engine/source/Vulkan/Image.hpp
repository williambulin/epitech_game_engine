#pragma once

#include "Extension/Libraries/Vulkan.hpp"
#include "Extension/Language/Library.hpp"

#include <stdexcept>
#include <string>

namespace Vulkan {
  class Image;
  class Swapchain;
};

class Vulkan::Image final {
private:
  enum class Type {
    UNDEFINED,
    SWAPCHAIN,
  };

public:
  Type             m_type{Type::UNDEFINED};
  std::uint32_t    m_mipLevels{1};
  VkImage          m_image{nullptr};
  VkDevice         m_device{nullptr};
  VkPhysicalDevice m_physicalDevice{nullptr};
  VkDeviceMemory   m_deviceMemory{nullptr};
  VkImageView      m_imageView{nullptr};
  VkSampler        m_sampler{nullptr};
  VkFormat         m_format{};
  VkCommandPool    m_commandPool{nullptr};
  VkQueue          m_graphicsQueue{nullptr};

private:
  [[nodiscard]] auto startSingleUseCommandBuffer() -> VkCommandBuffer;
  void               endSingleUseCommandBuffer(VkCommandBuffer commandBuffer);

  [[nodiscard]] bool          hasStrencilComponent(VkFormat format) const;
  [[nodiscard]] std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags memoryPropertyFlags) const;

  void               createImage(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkImage &image, VkDeviceMemory &deviceMemory, std::uint32_t mipLevels) const;
  [[nodiscard]] auto createImageView(VkImage image, VkFormat format, VkImageAspectFlags imageAspectFlags, std::uint32_t mipLevels = 1) const -> VkImageView;

  void generateMipmaps(VkImage image, VkFormat format, std::int32_t width, std::int32_t height, std::uint32_t mipLevels);
  void copyBufferToImage(VkBuffer buffer, VkImage image, std::uint32_t width, std::uint32_t height);

public:
  void transitionLayout(VkImageLayout imageLayout, VkImageLayout newImageLayout);

  DLLATTRIB explicit Image(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, const std::string &path);
  DLLATTRIB explicit Image(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkImageAspectFlags imageAspectFlags, std::uint32_t mipLevels = 1);
  DLLATTRIB explicit Image(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage image, const Swapchain &swapchain);
  DLLATTRIB ~Image();
};
