#pragma once

#include "Windows/Window.hpp"
#include "Extension/Libraries/Vulkan.hpp"
#include "QueueFamilies.hpp"
#include "SwapchainDetails.hpp"
#include "Swapchain.hpp"
#include "Model.hpp"
#include "ECS/Admin.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

namespace Vulkan {
  class Renderer;
}

class Vulkan::Renderer final {
private:
  static constexpr std::size_t maxFramesInFlight{1};

  const std::vector<const char *> m_extensions{
  VK_KHR_SURFACE_EXTENSION_NAME,
  VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
  VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
  };

  const std::vector<const char *> m_deviceExtensions{
  VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };

  const std::vector<const char *> m_validationLayers{
  "VK_LAYER_KHRONOS-validation",
  };

  ECS::Admin &     m_admin;
  Windows::Window &m_window;

  VkInstance               m_instance{nullptr};
  VkDebugUtilsMessengerEXT m_debugMessenger{nullptr};

  VkPhysicalDevice             m_physicalDevice{nullptr};
  VkDevice                     m_device{nullptr};
  VkSurfaceKHR                 m_surfaceKHR{nullptr};
  VkCommandPool                m_commandPool{nullptr};
  VkDescriptorSetLayout        m_descriptorSetLayout{nullptr};
  std::vector<VkCommandBuffer> m_commandBuffers{};

  VkQueue m_graphicsQueue{nullptr};
  VkQueue m_presentQueue{nullptr};

  std::vector<VkSemaphore> m_imageSemaphores{};
  std::vector<VkSemaphore> m_renderSemaphores{};
  std::vector<VkFence>     m_inFlightFences{};

  std::unique_ptr<Swapchain> m_swapchain{nullptr};

private:
  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT *debugUtilsMessengerCallbackDataEXT, void *);

  [[nodiscard]] bool checkValidationLayers() const;

  [[nodiscard]] auto findPhysicalDevice() const -> VkPhysicalDevice;

public:
  [[nodiscard]] auto createImage(const std::string &path) const -> std::unique_ptr<Image>;
  [[nodiscard]] auto createModel(const std::string &path, const ml::vec3 &color) const -> std::unique_ptr<Model>;

  void update(float deltatime, std::uint64_t tickCount);

  explicit Renderer(ECS::Admin &admin, Windows::Window &window);
  ~Renderer();
};
