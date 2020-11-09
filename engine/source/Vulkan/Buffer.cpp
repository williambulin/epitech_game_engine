#include "Buffer.hpp"
#include "Extension/Language/Keywords.hpp"

void Vulkan::Buffer::createBuffer(size_t bufferSize, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer &buffer, VkDeviceMemory &bufferDeviceMemory) {
  VkBufferCreateInfo bufferCreateInfo{
  .sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
  .size        = bufferSize,
  .usage       = bufferUsageFlags,
  .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
  };

  if (vkCreateBuffer(m_device, std::addressof(bufferCreateInfo), nullptr, std::addressof(buffer)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create buffer"};

  VkMemoryRequirements memoryRequirements{};
  vkGetBufferMemoryRequirements(m_device, buffer, std::addressof(memoryRequirements));

  VkMemoryAllocateInfo memoryAllocateInfo{
  .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
  .allocationSize  = memoryRequirements.size,
  .memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, memoryPropertyFlags),
  };

  if (vkAllocateMemory(m_device, std::addressof(memoryAllocateInfo), nullptr, std::addressof(bufferDeviceMemory)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't allocate memory for buffer"};

  vkBindBufferMemory(m_device, buffer, bufferDeviceMemory, 0);
}

auto Vulkan::Buffer::startSingleUseCommandBuffer() -> VkCommandBuffer {
  VkCommandBufferAllocateInfo commandBufferAllocateInfo{
  .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
  .commandPool        = m_commandPool,
  .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
  .commandBufferCount = 1,
  };

  VkCommandBuffer commandBuffer{nullptr};
  vkAllocateCommandBuffers(m_device, std::addressof(commandBufferAllocateInfo), std::addressof(commandBuffer));

  VkCommandBufferBeginInfo commandBufferBeginInfo{
  .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
  .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  vkBeginCommandBuffer(commandBuffer, std::addressof(commandBufferBeginInfo));
  return commandBuffer;
}

void Vulkan::Buffer::endSingleUseCommandBuffer(VkCommandBuffer commandBuffer) {
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{
  .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
  .commandBufferCount = 1,
  .pCommandBuffers    = std::addressof(commandBuffer),
  };

  vkQueueSubmit(m_graphicsQueue, 1, std::addressof(submitInfo), nullptr);
  vkQueueWaitIdle(m_graphicsQueue);

  vkFreeCommandBuffers(m_device, m_commandPool, 1, std::addressof(commandBuffer));
}

std::uint32_t Vulkan::Buffer::findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags memoryPropertyFlags) const {
  VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties{};
  vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, std::addressof(physicalDeviceMemoryProperties));

  for (std::uint32_t i{0}; i < physicalDeviceMemoryProperties.memoryTypeCount; ++i)
    if ((typeFilter & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
      return i;

  throw std::runtime_error{"Couldn't find suitable memory type"};
}

void Vulkan::Buffer::buffercpy(VkBuffer src, VkBuffer dst, VkDeviceSize deviceSize) {
  using (auto commandBuffer{startSingleUseCommandBuffer()}) {
    VkBufferCopy bufferCopy{
    .size = deviceSize,
    };

    vkCmdCopyBuffer(commandBuffer, src, dst, 1, std::addressof(bufferCopy));

    endSingleUseCommandBuffer(commandBuffer);
  }
}

void Vulkan::Buffer::buffercpy(VkBuffer src, VkDeviceSize deviceSize) {
  buffercpy(src, m_buffer, deviceSize);
}

void Vulkan::Buffer::cpy(void *src, std::size_t size) {
  void *data{nullptr};
  vkMapMemory(m_device, m_deviceMemory, 0, size, 0, std::addressof(data));
  std::memcpy(data, src, size);
  vkUnmapMemory(m_device, m_deviceMemory);
}

Vulkan::Buffer::Buffer(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, size_t bufferSize, VkBufferUsageFlags bufferUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags) : m_physicalDevice{physicalDevice}, m_device{device}, m_commandPool{commandPool}, m_graphicsQueue{graphicsQueue} {
  createBuffer(bufferSize, bufferUsageFlags, memoryPropertyFlags, m_buffer, m_deviceMemory);
}

Vulkan::Buffer::~Buffer() {
  if (m_deviceMemory != nullptr)
    vkFreeMemory(m_device, m_deviceMemory, nullptr);

  if (m_buffer != nullptr)
    vkDestroyBuffer(m_device, m_buffer, nullptr);
}
