#pragma once

#include "Extension/Libraries/Vulkan.hpp"
#include "Vulkan/QueueFamilies.hpp"
#include "Vulkan/SwapchainDetails.hpp"
#include "Vulkan/Image.hpp"
#include "Vulkan/Buffer.hpp"
#include "Vulkan/Vertex.hpp"

#include <memory>
#include <stdexcept>
#include <fstream>
#include <iostream>

namespace Vulkan {
  class Swapchain;
}

class Vulkan::Swapchain final {
public:
  VkPhysicalDevice m_physicalDevice{nullptr};
  VkDevice         m_device{nullptr};
  VkCommandPool    m_commandPool{nullptr};
  VkQueue          m_graphicsQueue{nullptr};

  VkExtent2D m_extent2D{};
  VkFormat   m_format{};

  VkSwapchainKHR                       m_swapchain{nullptr};
  VkRenderPass                         m_renderPass{nullptr};
  VkPipelineLayout                     m_pipelineLayout{nullptr};
  VkPipeline                           m_graphicsPipeline{nullptr};
  std::vector<VkFramebuffer>           m_framebuffers{};
  std::unique_ptr<Image>               m_depthImage{nullptr};
  std::vector<std::unique_ptr<Image>>  m_images{};
  std::vector<std::unique_ptr<Buffer>> m_uniformBuffers{};
  VkDescriptorPool                     m_descriptorPool{nullptr};
  std::vector<VkFence>                 m_imageFences{};
  std::vector<VkDescriptorSet>         m_descriptorSets{};

public:
  [[nodiscard]] auto getExtend2D() const noexcept -> const VkExtent2D &;
  [[nodiscard]] auto getFormat() const noexcept -> const VkFormat &;

  [[nodiscard]] auto findSupportedFormat(const std::vector<VkFormat> &formats, VkImageTiling imageTiling, VkFormatFeatureFlags formatFeatureFlags) const -> VkFormat;
  [[nodiscard]] auto findDepthFormat() const -> VkFormat;

  [[nodiscard]] auto loadShader(const std::string &path) const -> std::vector<char>;
  [[nodiscard]] auto createShader(const std::vector<char> &bytes) const -> VkShaderModule;

  explicit Swapchain(VkCommandPool commandPool, VkQueue graphicsQueue, HWND hwnd, const VkPhysicalDevice physicalDevice, const VkDevice device, const VkSurfaceKHR surfaceKHR, const VkDescriptorSetLayout descriptorSetLayout, const QueueFamilies &queueFamilies);
  ~Swapchain();
};
