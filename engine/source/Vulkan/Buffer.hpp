#pragma once

#include "Extension/Libraries/Vulkan.hpp"
#include "Extension/Language/Library.hpp"

#include <stdexcept>

namespace Vulkan {
  class Buffer;
}

class Vulkan::Buffer final {
public:
  VkPhysicalDevice m_physicalDevice{nullptr};
  VkDevice         m_device{nullptr};
  VkCommandPool    m_commandPool{nullptr};
  VkQueue          m_graphicsQueue{nullptr};

  VkBuffer       m_buffer{nullptr};
  VkDeviceMemory m_deviceMemory{nullptr};

private:
  [[nodiscard]] auto          startSingleUseCommandBuffer() -> VkCommandBuffer;
  void                        endSingleUseCommandBuffer(VkCommandBuffer commandBuffer);
  [[nodiscard]] std::uint32_t findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags memoryPropertyFlags) const;
  void                        createBuffer(size_t bufferSize, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer &buffer, VkDeviceMemory &bufferDeviceMemory);
  void                        buffercpy(VkBuffer src, VkBuffer dst, VkDeviceSize deviceSize);

public:
  void buffercpy(VkBuffer src, VkDeviceSize size);
  void cpy(void *src, std::size_t size);

  DLLATTRIB explicit Buffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, size_t bufferSize, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags);
  DLLATTRIB ~Buffer();
};
