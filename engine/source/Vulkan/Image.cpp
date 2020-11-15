#include "Image.hpp"

#include "Vulkan/Swapchain.hpp"
#include "Vulkan/Buffer.hpp"
#include "Extension/Language/Keywords.hpp"
#include "Extension/Libraries/stb_image.h"

auto Vulkan::Image::startSingleUseCommandBuffer() -> VkCommandBuffer {
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

void Vulkan::Image::endSingleUseCommandBuffer(VkCommandBuffer commandBuffer) {
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

bool Vulkan::Image::hasStrencilComponent(VkFormat format) const {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

std::uint32_t Vulkan::Image::findMemoryType(std::uint32_t typeFilter, VkMemoryPropertyFlags memoryPropertyFlags) const {
  VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties{};
  vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, std::addressof(physicalDeviceMemoryProperties));

  for (std::uint32_t i{0}; i < physicalDeviceMemoryProperties.memoryTypeCount; ++i)
    if ((typeFilter & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags)
      return i;

  throw std::runtime_error{"Couldn't find suitable memory type"};
}

void Vulkan::Image::createImage(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkImage &image, VkDeviceMemory &deviceMemory, std::uint32_t mipLevels) const {
  VkImageCreateInfo imageCreateInfo{
  .sType     = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
  .imageType = VK_IMAGE_TYPE_2D,
  .format    = format,
  .extent =
  VkExtent3D{
  .width  = width,
  .height = height,
  .depth  = 1,
  },
  .mipLevels     = mipLevels,
  .arrayLayers   = 1,
  .samples       = VK_SAMPLE_COUNT_1_BIT,
  .tiling        = imageTiling,
  .usage         = imageUsageFlags,
  .sharingMode   = VK_SHARING_MODE_EXCLUSIVE,
  .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
  };

  if (vkCreateImage(m_device, std::addressof(imageCreateInfo), nullptr, std::addressof(image)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create image"};

  VkMemoryRequirements memoryRequirements{};
  vkGetImageMemoryRequirements(m_device, image, std::addressof(memoryRequirements));

  VkMemoryAllocateInfo memoryAllocateInfo{
  .sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
  .allocationSize  = memoryRequirements.size,
  .memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, memoryPropertyFlags),
  };

  if (vkAllocateMemory(m_device, std::addressof(memoryAllocateInfo), nullptr, std::addressof(deviceMemory)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't allocate memory for image"};

  vkBindImageMemory(m_device, image, deviceMemory, 0);
}

auto Vulkan::Image::createImageView(VkImage image, VkFormat format, VkImageAspectFlags imageAspectFlags, std::uint32_t mipLevels) const -> VkImageView {
  VkImageView imageView{nullptr};

  VkImageViewCreateInfo imageViewCreateInfo{
  .sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
  .image    = image,
  .viewType = VK_IMAGE_VIEW_TYPE_2D,
  .format   = format,
  .components =
  VkComponentMapping{
  .r = VK_COMPONENT_SWIZZLE_IDENTITY,
  .g = VK_COMPONENT_SWIZZLE_IDENTITY,
  .b = VK_COMPONENT_SWIZZLE_IDENTITY,
  .a = VK_COMPONENT_SWIZZLE_IDENTITY,
  },
  .subresourceRange =
  VkImageSubresourceRange{
  .aspectMask     = imageAspectFlags,
  .baseMipLevel   = 0,
  .levelCount     = mipLevels,
  .baseArrayLayer = 0,
  .layerCount     = 1,
  },
  };

  if (vkCreateImageView(m_device, std::addressof(imageViewCreateInfo), nullptr, std::addressof(imageView)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create image view"};

  return imageView;
}

void Vulkan::Image::copyBufferToImage(VkBuffer buffer, VkImage image, std::uint32_t width, std::uint32_t height) {
  use(auto commandBuffer{startSingleUseCommandBuffer()}) {
    VkBufferImageCopy bufferImageCopy{
    .bufferOffset      = 0,
    .bufferRowLength   = 0,
    .bufferImageHeight = 0,
    .imageSubresource =
    VkImageSubresourceLayers{
    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
    .mipLevel       = 0,
    .baseArrayLayer = 0,
    .layerCount     = 1,
    },
    .imageOffset = {0, 0, 0},
    .imageExtent = {width, height, 1},
    };

    vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, std::addressof(bufferImageCopy));

    endSingleUseCommandBuffer(commandBuffer);
  }
}

void Vulkan::Image::transitionLayout(VkImageLayout imageLayout, VkImageLayout newImageLayout) {
  use(auto commandBuffer{startSingleUseCommandBuffer()}) {
    VkImageMemoryBarrier imageMemoryBarrier{
    .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .srcAccessMask       = 0,
    .dstAccessMask       = 0,
    .oldLayout           = imageLayout,
    .newLayout           = newImageLayout,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image               = m_image,
    .subresourceRange =
    VkImageSubresourceRange{
    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
    .baseMipLevel   = 0,
    .levelCount     = m_mipLevels,
    .baseArrayLayer = 0,
    .layerCount     = 1,
    },
    };

    VkPipelineStageFlags pipelineStageFlagsSource{};
    VkPipelineStageFlags pipelineStageFlagsDestination{};

    if (newImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
      imageMemoryBarrier.subresourceRange.aspectMask = hasStrencilComponent(m_format) ? VK_IMAGE_ASPECT_STENCIL_BIT : VK_IMAGE_ASPECT_DEPTH_BIT;

    if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newImageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
      imageMemoryBarrier.srcAccessMask = 0;
      imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      pipelineStageFlagsSource         = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      pipelineStageFlagsDestination    = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (imageLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newImageLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
      imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      pipelineStageFlagsSource         = VK_PIPELINE_STAGE_TRANSFER_BIT;
      pipelineStageFlagsDestination    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (imageLayout == VK_IMAGE_LAYOUT_UNDEFINED && newImageLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      imageMemoryBarrier.srcAccessMask = 0;
      imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      pipelineStageFlagsSource         = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      pipelineStageFlagsDestination    = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } else
      throw std::runtime_error{"Couldn't transition unsupported layouts"};

    vkCmdPipelineBarrier(commandBuffer, pipelineStageFlagsSource, pipelineStageFlagsDestination, 0, 0, nullptr, 0, nullptr, 1, std::addressof(imageMemoryBarrier));

    endSingleUseCommandBuffer(commandBuffer);
  }
}

void Vulkan::Image::generateMipmaps(VkImage image, VkFormat format, std::int32_t width, std::int32_t height, std::uint32_t mipLevels) {
  VkFormatProperties formatProperties{};
  vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, std::addressof(formatProperties));

  if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT))
    throw std::runtime_error{"Format doesn't support linear filtering"};

  use(auto commandBuffer{startSingleUseCommandBuffer()}) {
    VkImageMemoryBarrier imageMemoryBarrier{
    .sType               = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
    .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
    .image               = image,
    .subresourceRange =
    VkImageSubresourceRange{
    .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
    .levelCount     = 1,
    .baseArrayLayer = 0,
    .layerCount     = 1,
    },
    };

    std::int32_t mipWidth{width};
    std::int32_t mipHeight{height};
    for (std::uint32_t i{1}; i < mipLevels; ++i) {
      imageMemoryBarrier.subresourceRange.baseMipLevel = i - 1;
      imageMemoryBarrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      imageMemoryBarrier.newLayout                     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      imageMemoryBarrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
      imageMemoryBarrier.dstAccessMask                 = VK_ACCESS_TRANSFER_READ_BIT;

      vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, std::addressof(imageMemoryBarrier));

      VkImageBlit imageBlit{
      .srcSubresource =
      {
      .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
      .mipLevel       = i - 1,
      .baseArrayLayer = 0,
      .layerCount     = 1,
      },
      .srcOffsets =
      {
      {0, 0, 0},
      {mipWidth, mipHeight, 1},
      },
      .dstSubresource =
      {
      .aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT,
      .mipLevel       = i,
      .baseArrayLayer = 0,
      .layerCount     = 1,
      },
      .dstOffsets =
      {
      {0, 0, 0},
      {((mipWidth > 1) ? (mipWidth / 2) : 1), ((mipHeight > 1) ? (mipHeight / 2) : 1), 1},
      },
      };

      vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, std::addressof(imageBlit), VK_FILTER_LINEAR);

      imageMemoryBarrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      imageMemoryBarrier.newLayout     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, std::addressof(imageMemoryBarrier));

      if (mipWidth > 1)
        mipWidth /= 2;
      if (mipHeight > 1)
        mipHeight /= 2;
    }

    imageMemoryBarrier.subresourceRange.baseMipLevel = mipLevels - 1;
    imageMemoryBarrier.oldLayout                     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    imageMemoryBarrier.newLayout                     = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageMemoryBarrier.srcAccessMask                 = VK_ACCESS_TRANSFER_WRITE_BIT;
    imageMemoryBarrier.dstAccessMask                 = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, std::addressof(imageMemoryBarrier));

    endSingleUseCommandBuffer(commandBuffer);
  }
}

#undef max

Vulkan::Image::Image(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, const std::string &path) : m_device{device}, m_physicalDevice{physicalDevice}, m_format{VK_FORMAT_R8G8B8A8_SRGB}, m_commandPool{commandPool}, m_graphicsQueue{graphicsQueue} {
  int  imageWidth{0}, imageHeight{0}, imageChannels{0};
  auto pixels{stbi_load(path.c_str(), std::addressof(imageWidth), std::addressof(imageHeight), std::addressof(imageChannels), STBI_rgb_alpha)};
  if (pixels == nullptr)
    throw std::runtime_error{"Couldn't load texture"};

  m_mipLevels = static_cast<std::uint32_t>(std::floor(std::log2(std::max(imageWidth, imageHeight)))) + 1;

  VkDeviceSize size{static_cast<VkDeviceSize>(imageWidth * imageHeight * STBI_rgb_alpha)};

  Buffer stagingBuffer{m_physicalDevice, m_device, m_commandPool, m_graphicsQueue, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT};
  stagingBuffer.cpy(pixels, size);

  stbi_image_free(pixels);

  createImage(static_cast<std::uint32_t>(imageWidth), static_cast<std::uint32_t>(imageHeight), VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_image, m_deviceMemory, m_mipLevels);
  transitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  copyBufferToImage(stagingBuffer.m_buffer, m_image, static_cast<std::uint32_t>(imageWidth), static_cast<std::uint32_t>(imageHeight));
  generateMipmaps(m_image, VK_FORMAT_R8G8B8A8_SRGB, imageWidth, imageHeight, m_mipLevels);

  m_imageView = createImageView(m_image, m_format, VK_IMAGE_ASPECT_COLOR_BIT, m_mipLevels);

  VkSamplerCreateInfo samplerCreateInfo{
  .sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
  .magFilter               = VK_FILTER_LINEAR,
  .minFilter               = VK_FILTER_LINEAR,
  .mipmapMode              = VK_SAMPLER_MIPMAP_MODE_LINEAR,
  .addressModeU            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
  .addressModeV            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
  .addressModeW            = VK_SAMPLER_ADDRESS_MODE_REPEAT,
  .mipLodBias              = 0.0f,
  .anisotropyEnable        = VK_TRUE,  // VK_FALSE when off
  .maxAnisotropy           = 16.0f,    // 1.0f when off
  .compareEnable           = VK_FALSE,
  .compareOp               = VK_COMPARE_OP_ALWAYS,
  .minLod                  = 0.0f,
  .maxLod                  = static_cast<float>(m_mipLevels),
  .borderColor             = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
  .unnormalizedCoordinates = VK_FALSE,
  };

  if (vkCreateSampler(m_device, std::addressof(samplerCreateInfo), nullptr, std::addressof(m_sampler)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create texture sampler"};
}

Vulkan::Image::Image(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkImageAspectFlags imageAspectFlags, std::uint32_t mipLevels)
  : m_mipLevels{mipLevels},
    m_device{device},
    m_physicalDevice{physicalDevice},
    m_format{format},
    m_commandPool{commandPool},
    m_graphicsQueue{graphicsQueue} {
  createImage(width, height, m_format, imageTiling, imageUsageFlags, memoryPropertyFlags, m_image, m_deviceMemory, m_mipLevels);
  m_imageView = createImageView(m_image, m_format, imageAspectFlags);
}

Vulkan::Image::Image(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue graphicsQueue, VkImage image, const Swapchain &swapchain) : m_type{Type::SWAPCHAIN}, m_image{image}, m_device{device}, m_physicalDevice{physicalDevice}, m_format{swapchain.getFormat()}, m_commandPool{commandPool}, m_graphicsQueue{graphicsQueue} {
  m_imageView = createImageView(m_image, m_format, VK_IMAGE_ASPECT_COLOR_BIT);
}

Vulkan::Image::~Image() {
  if (m_sampler != nullptr)
    vkDestroySampler(m_device, m_sampler, nullptr);

  if (m_deviceMemory != nullptr)
    vkFreeMemory(m_device, m_deviceMemory, nullptr);

  if (m_imageView != nullptr)
    vkDestroyImageView(m_device, m_imageView, nullptr);

  if (m_image != nullptr && m_type != Type::SWAPCHAIN)
    vkDestroyImage(m_device, m_image, nullptr);
}
