#include "Renderer.hpp"
#include "Model.hpp"
#include "UniformBufferData.hpp"
#include "Drawable.hpp"
#include "Components/Transform.hpp"
#include "Components/Model.hpp"

#include <chrono>

VKAPI_ATTR VkBool32 VKAPI_CALL Vulkan::Renderer::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT *debugUtilsMessengerCallbackDataEXT, void *) {
  std::cout << debugUtilsMessengerCallbackDataEXT->pMessage << '\n' << '\n';
  return VK_FALSE;
}

bool Vulkan::Renderer::checkValidationLayers() const {
  std::uint32_t propertyCount{0};
  vkEnumerateInstanceLayerProperties(std::addressof(propertyCount), nullptr);

  std::vector<VkLayerProperties> properties(propertyCount);
  vkEnumerateInstanceLayerProperties(std::addressof(propertyCount), properties.data());

  return std::ranges::all_of(m_validationLayers, [&properties](const auto &validationLayer) {
    return std::ranges::any_of(properties, [&validationLayer](const auto &property) {
      return std::strcmp(validationLayer, property.layerName) == 0;
    });
  });
}

auto Vulkan::Renderer::findPhysicalDevice() const -> VkPhysicalDevice {
  std::uint32_t physicalDeviceCount{0};
  vkEnumeratePhysicalDevices(m_instance, std::addressof(physicalDeviceCount), nullptr);
  std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
  vkEnumeratePhysicalDevices(m_instance, std::addressof(physicalDeviceCount), physicalDevices.data());

  auto result{std::ranges::find_if(physicalDevices, [this](auto &physicalDevice) {
    if (physicalDevice == nullptr)
      return false;

    QueueFamilies queueFamilies{physicalDevice, m_surfaceKHR};
    if (!queueFamilies.isSuitable())
      return false;

    std::uint32_t deviceExtensionPropertiesCount{0};
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, std::addressof(deviceExtensionPropertiesCount), nullptr);
    std::vector<VkExtensionProperties> deviceExtensionProperties(deviceExtensionPropertiesCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, std::addressof(deviceExtensionPropertiesCount), deviceExtensionProperties.data());

    auto hasRequiredExtensions{std::ranges::all_of(m_deviceExtensions, [&deviceExtensionProperties](const auto &deviceExtension) {
      return std::ranges::any_of(deviceExtensionProperties, [&deviceExtension](const auto &a) {
        return std::strcmp(a.extensionName, deviceExtension) == 0;
      });
    })};

    if (!hasRequiredExtensions)
      return false;

    SwapchainDetails swapChainDetails{physicalDevice, m_surfaceKHR};
    if (swapChainDetails.m_formats.empty() || swapChainDetails.m_presentModes.empty())
      return false;

    VkPhysicalDeviceFeatures physicalDeviceFeatures{};
    vkGetPhysicalDeviceFeatures(physicalDevice, std::addressof(physicalDeviceFeatures));

    if (!physicalDeviceFeatures.samplerAnisotropy)
      return false;

    return true;
  })};

  return result != physicalDevices.end() ? *result : nullptr;
}

auto Vulkan::Renderer::createImage(const std::string &path) const -> std::unique_ptr<Image> {
  return std::make_unique<Vulkan::Image>(m_device, m_physicalDevice, m_commandPool, m_graphicsQueue, path);
}

auto Vulkan::Renderer::createModel(const std::string &path) const -> std::unique_ptr<Model> {
  return std::make_unique<Vulkan::Model>(m_physicalDevice, m_device, m_commandPool, m_graphicsQueue, path);
}

void Vulkan::Renderer::update(float, std::uint64_t) {
  auto drawableEntities{m_admin.getEntitiesWithComponents<Components::Transform, Components::Model>()};

  static std::size_t currentFrame{0};

  // Render Loop
  vkWaitForFences(m_device, 1, std::addressof(m_inFlightFences[currentFrame]), VK_TRUE, UINT64_MAX);

  std::uint32_t imageIndex{0};
  vkAcquireNextImageKHR(m_device, m_swapchain->m_swapchain, UINT64_MAX, m_imageSemaphores[currentFrame], nullptr, std::addressof(imageIndex));

  if (m_swapchain->m_imageFences[imageIndex] != nullptr)
    vkWaitForFences(m_device, 1, std::addressof(m_swapchain->m_imageFences[imageIndex]), VK_TRUE, UINT64_MAX);

  m_swapchain->m_imageFences[imageIndex] = m_inFlightFences[currentFrame];

  // Update uniform buffers
  UniformBufferData ubo{
  .model            = ml::mat4{},
  .view             = ml::mat4::lookAt(ml::vec3{0.0f, 0.01f, 0.5f}, ml::vec3{0.0f, 0.0f, 0.0f}, ml::vec3{0.0f, 0.0f, 1.0f}),
  .projection       = ml::mat4::perspective(glm::radians(80.0f), static_cast<float>(m_swapchain->m_extent2D.width) / static_cast<float>(m_swapchain->m_extent2D.height), 0.001f, 1000.0f),  // glm::perspective(glm::radians(45.0f), static_cast<float>(m_swapchain->m_extent2D.width) / static_cast<float>(m_swapchain->m_extent2D.height), 0.1f, 10.0f),
  .lightSourceCount = 0.0f,
  };

  std::size_t i{0};
  auto        entities{m_admin.getEntitiesWithComponents<Components::Light, Components::Transform>()};
  for (auto &&[entity, light, transform] : entities) {
    if (i >= (sizeof(ubo.lightSource) / sizeof(Components::Light::GLSLStruct)))
      break;

    ubo.lightSource[i]          = light.toGLSLStruct();
    ubo.lightSource[i].position = transform.matrix.getTranslation();
    // if (ubo.lightSource[i].type == Components::Light::Type::Directional)
    ubo.lightSource[i].direction = {transform.matrix.getRotation() * light.direction};
    ++ubo.lightSourceCount;
    ++i;
  }

  ubo.projection[1][1] *= -1;

  std::uint32_t currentObject{0};
  for (auto &&[ent, transform, model] : drawableEntities) {
    // Update the data in the Uniform Buffer
    ubo.model = transform.matrix;

    m_swapchain->m_uniformBuffers[currentObject]->cpy(std::addressof(ubo), sizeof(ubo));

    // Update the descriptor set
    VkDescriptorBufferInfo descriptorBufferInfo{
    .buffer = m_swapchain->m_uniformBuffers[currentObject]->m_buffer,
    .offset = 0,
    .range  = sizeof(UniformBufferData),
    };

    VkDescriptorImageInfo descriptorImageInfo{
    .sampler     = model.m_image->m_sampler,
    .imageView   = model.m_image->m_imageView,
    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    };

    VkWriteDescriptorSet writeDescriptorSetUBO{
    .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .dstSet          = m_swapchain->m_descriptorSets[currentObject],
    .dstBinding      = 0,
    .dstArrayElement = 0,
    .descriptorCount = 1,
    .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    .pBufferInfo     = std::addressof(descriptorBufferInfo),
    };

    VkWriteDescriptorSet writeDescriptorSetTextureSampler{
    .sType           = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .dstSet          = m_swapchain->m_descriptorSets[currentObject],
    .dstBinding      = 1,
    .dstArrayElement = 0,
    .descriptorCount = 1,
    .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    .pImageInfo      = std::addressof(descriptorImageInfo),
    };

    std::vector<VkWriteDescriptorSet> writeDescriptorSets{writeDescriptorSetUBO, writeDescriptorSetTextureSampler};

    vkUpdateDescriptorSets(m_device, static_cast<std::uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

    ++currentObject;
  }

  // Command buffers
  for (auto it{m_commandBuffers.begin()}; it != m_commandBuffers.end(); ++it) {
    auto index{std::distance(m_commandBuffers.begin(), it)};
    if (index != imageIndex)
      continue;

    VkCommandBufferBeginInfo commandBufferBeginInfo{
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    };

    vkResetCommandBuffer(*it, 0);

    if (vkBeginCommandBuffer(*it, std::addressof(commandBufferBeginInfo)) != VK_SUCCESS)
      throw std::runtime_error{"Couldn't begin command buffer"};

    std::vector<VkClearValue> clearValues{
    {.color = {{0.0f, 0.0f, 0.0f, 1.0f}}},
    {.depthStencil = {1.0f, 0}},
    };

    VkRenderPassBeginInfo renderPassBeginInfo{
    .sType       = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
    .renderPass  = m_swapchain->m_renderPass,
    .framebuffer = m_swapchain->m_framebuffers[static_cast<std::size_t>(index)],  // TODO: Change me
    .renderArea =
    VkRect2D{
    .offset = {0, 0},
    .extent = m_swapchain->m_extent2D,
    },
    .clearValueCount = static_cast<std::uint32_t>(clearValues.size()),
    .pClearValues    = clearValues.data(),
    };

    vkCmdBeginRenderPass(*it, std::addressof(renderPassBeginInfo), VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(*it, VK_PIPELINE_BIND_POINT_GRAPHICS, m_swapchain->m_graphicsPipeline);

    // vkCmdPushConstants(*it, m_swapchain->m_pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(LightSource), std::addressof(wow));

    currentObject = 0;
    for (auto &&[ent, transform, model] : drawableEntities) {
      std::vector<VkBuffer>     buffers{model.m_model->m_vertexBuffer->m_buffer};
      std::vector<VkDeviceSize> offsets{0};
      vkCmdBindVertexBuffers(*it, 0, 1, buffers.data(), offsets.data());
      vkCmdBindIndexBuffer(*it, model.m_model->m_indicesBuffer->m_buffer, 0, VK_INDEX_TYPE_UINT32);
      vkCmdBindDescriptorSets(*it, VK_PIPELINE_BIND_POINT_GRAPHICS, m_swapchain->m_pipelineLayout, 0, 1, std::addressof(m_swapchain->m_descriptorSets[currentObject]), 0, nullptr);
      vkCmdDrawIndexed(*it, static_cast<std::uint32_t>(model.m_model->m_indicesList.size()), 1, 0, 0, 0);

      ++currentObject;
    }

    vkCmdEndRenderPass(*it);

    if (vkEndCommandBuffer(*it) != VK_SUCCESS)
      throw std::runtime_error{"Couldn't end command buffer " + std::to_string(index)};
  }

  // Submit info
  std::vector<VkPipelineStageFlags> pipelineStageFlags{VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

  VkSubmitInfo submitInfo{
  .sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO,
  .waitSemaphoreCount   = 1,
  .pWaitSemaphores      = std::addressof(m_imageSemaphores[currentFrame]),
  .pWaitDstStageMask    = pipelineStageFlags.data(),
  .commandBufferCount   = 1,
  .pCommandBuffers      = std::addressof(m_commandBuffers[imageIndex]),
  .signalSemaphoreCount = 1,
  .pSignalSemaphores    = std::addressof(m_renderSemaphores[currentFrame]),
  };

  vkResetFences(m_device, 1, std::addressof(m_inFlightFences[currentFrame]));

  if (vkQueueSubmit(m_graphicsQueue, 1, std::addressof(submitInfo), m_inFlightFences[currentFrame]) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't submit queue"};

  VkPresentInfoKHR presentInfo{
  .sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
  .waitSemaphoreCount = 1,
  .pWaitSemaphores    = std::addressof(m_renderSemaphores[currentFrame]),
  .swapchainCount     = 1,
  .pSwapchains        = std::addressof(m_swapchain->m_swapchain),
  .pImageIndices      = std::addressof(imageIndex),
  };

  auto result{vkQueuePresentKHR(m_graphicsQueue, std::addressof(presentInfo))};
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    m_swapchain.reset();
    QueueFamilies queueFamilies{m_physicalDevice, m_surfaceKHR};
    m_swapchain = std::make_unique<Swapchain>(m_commandPool, m_graphicsQueue, m_window.getHandle(), m_physicalDevice, m_device, m_surfaceKHR, m_descriptorSetLayout, queueFamilies);
  } else if (result != VK_SUCCESS)
    throw std::runtime_error{"Couldn't present SwapChain image"};

  ++currentFrame;
  currentFrame = currentFrame % maxFramesInFlight;
}

Vulkan::Renderer::Renderer(ECS::Admin &admin, Windows::Window &window) : m_admin{admin}, m_window{window} {
  VkApplicationInfo applicationInfo{
  .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
  .pApplicationName   = "Vulkan",
  .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
  .pEngineName        = "Vulkan",
  .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
  .apiVersion         = VK_API_VERSION_1_0,
  };

  VkInstanceCreateInfo instanceCreateInfo{
  .sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
  .pNext                   = nullptr,
  .pApplicationInfo        = std::addressof(applicationInfo),
  .enabledLayerCount       = 0,
  .enabledExtensionCount   = static_cast<std::uint32_t>(m_extensions.size()),
  .ppEnabledExtensionNames = m_extensions.data(),
  };

  VkDebugUtilsMessengerCreateInfoEXT debugMessengerCreateInfo{
  .sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
  .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
  .messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
  .pfnUserCallback = debugCallback,
  };

  bool validationLayers{checkValidationLayers()};
  if (validationLayers) {
    instanceCreateInfo.enabledLayerCount   = static_cast<std::uint32_t>(m_validationLayers.size());
    instanceCreateInfo.ppEnabledLayerNames = m_validationLayers.data();
    instanceCreateInfo.pNext               = std::addressof(debugMessengerCreateInfo);
  }

  if (vkCreateInstance(std::addressof(instanceCreateInfo), nullptr, std::addressof(m_instance)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create VkInstance"};

  if (validationLayers) {
    auto vkCreateDebugUtilsMessengerEXT{reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"))};
    if (vkCreateDebugUtilsMessengerEXT == nullptr)
      throw std::runtime_error{"Couldn't get memory location for vkCreateDebugUtilsMessengerEXT"};

    if (vkCreateDebugUtilsMessengerEXT(m_instance, std::addressof(debugMessengerCreateInfo), nullptr, std::addressof(m_debugMessenger)) != VK_SUCCESS)
      throw std::runtime_error{"Couldn't create debug messenger"};
  }

  // Surface
  VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{
  .sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
  .hinstance = GetModuleHandle(nullptr),
  .hwnd      = m_window.getHandle(),
  };

  if (vkCreateWin32SurfaceKHR(m_instance, std::addressof(surfaceCreateInfo), nullptr, std::addressof(m_surfaceKHR)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create window surface"};

  // Physical device
  m_physicalDevice = findPhysicalDevice();
  if (m_physicalDevice == nullptr)
    throw std::runtime_error{"Couldn't find a suitable physical device"};

  // Queues
  QueueFamilies queueFamilies{m_physicalDevice, m_surfaceKHR};
  if (!queueFamilies.isSuitable())
    throw std::runtime_error{"The physical device isn't suitable"};

  auto queueFamiliesCreateInfo{queueFamilies.getQueueFamiliesCreateInfo()};

  // Device
  VkPhysicalDeviceFeatures physicalDeviceFeatures{
  .samplerAnisotropy = VK_TRUE,
  };

  VkDeviceCreateInfo deviceCreateInfo{
  .sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
  .queueCreateInfoCount    = static_cast<std::uint32_t>(queueFamiliesCreateInfo.size()),
  .pQueueCreateInfos       = queueFamiliesCreateInfo.data(),
  .enabledExtensionCount   = static_cast<std::uint32_t>(m_deviceExtensions.size()),
  .ppEnabledExtensionNames = m_deviceExtensions.data(),
  .pEnabledFeatures        = std::addressof(physicalDeviceFeatures),
  };

  if (vkCreateDevice(m_physicalDevice, std::addressof(deviceCreateInfo), nullptr, std::addressof(m_device)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create logical device"};

  // Get queues
  vkGetDeviceQueue(m_device, queueFamilies.m_graphicsQueue.value(), 0, std::addressof(m_graphicsQueue));
  vkGetDeviceQueue(m_device, queueFamilies.m_presentQueue.value(), 0, std::addressof(m_presentQueue));

  // Semaphores and Fences
  VkSemaphoreCreateInfo semaphoreCreateInfo{
  .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
  };

  VkFenceCreateInfo fenceCreateInfo{
  .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
  .flags = VK_FENCE_CREATE_SIGNALED_BIT,
  };

  m_imageSemaphores.resize(maxFramesInFlight, nullptr);
  m_renderSemaphores.resize(maxFramesInFlight, nullptr);
  m_inFlightFences.resize(maxFramesInFlight, nullptr);

  for (std::size_t i{0}; i < maxFramesInFlight; ++i) {
    if (vkCreateSemaphore(m_device, std::addressof(semaphoreCreateInfo), nullptr, std::addressof(m_imageSemaphores[i])) != VK_SUCCESS)
      throw std::runtime_error{"Couldn't create image semaphore"};

    if (vkCreateSemaphore(m_device, std::addressof(semaphoreCreateInfo), nullptr, std::addressof(m_renderSemaphores[i])) != VK_SUCCESS)
      throw std::runtime_error{"Couldn't create render semaphore"};

    if (vkCreateFence(m_device, std::addressof(fenceCreateInfo), nullptr, std::addressof(m_inFlightFences[i])) != VK_SUCCESS)
      throw std::runtime_error{"Couldn't create fence"};
  }

  // Command pool/queues
  VkCommandPoolCreateInfo commandPoolCreateInfo{
  .sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
  .flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
  .queueFamilyIndex = queueFamilies.m_graphicsQueue.value(),
  };

  if (vkCreateCommandPool(m_device, std::addressof(commandPoolCreateInfo), nullptr, std::addressof(m_commandPool)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create command pool"};

  // Uniform Buffer Objects (UBO)
  VkDescriptorSetLayoutBinding descriptorSetLayoutBindingUBO{
  .binding         = 0,
  .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
  .descriptorCount = 1,
  .stageFlags      = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
  };

  VkDescriptorSetLayoutBinding descriptorSetLayoutBindingTextureSampler{
  .binding         = 1,
  .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
  .descriptorCount = 1,
  .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
  };

  std::vector<VkDescriptorSetLayoutBinding> bindings{descriptorSetLayoutBindingUBO, descriptorSetLayoutBindingTextureSampler};

  VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo{
  .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
  .bindingCount = static_cast<std::uint32_t>(bindings.size()),
  .pBindings    = bindings.data(),
  };

  if (vkCreateDescriptorSetLayout(m_device, std::addressof(descriptorSetLayoutCreateInfo), nullptr, std::addressof(m_descriptorSetLayout)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create descriptor set layout"};

  // Swapchain
  m_swapchain = std::make_unique<Swapchain>(m_commandPool, m_graphicsQueue, m_window.getHandle(), m_physicalDevice, m_device, m_surfaceKHR, m_descriptorSetLayout, queueFamilies);

  // Command buffers
  m_commandBuffers.resize(m_swapchain->m_framebuffers.size(), nullptr);

  VkCommandBufferAllocateInfo commandBufferAllocateInfo{
  .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
  .commandPool        = m_commandPool,
  .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
  .commandBufferCount = static_cast<std::uint32_t>(m_commandBuffers.size()),
  };

  if (vkAllocateCommandBuffers(m_device, std::addressof(commandBufferAllocateInfo), m_commandBuffers.data()) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't allocate command buffers"};
}

Vulkan::Renderer::~Renderer() {
  if (m_commandPool != nullptr)
    vkFreeCommandBuffers(m_device, m_commandPool, static_cast<std::uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());

  m_swapchain.reset();

  if (m_descriptorSetLayout != nullptr)
    vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);

  if (m_commandPool != nullptr)
    vkDestroyCommandPool(m_device, m_commandPool, nullptr);

  for (const auto &imageSemaphore : m_imageSemaphores)
    if (imageSemaphore != nullptr)
      vkDestroySemaphore(m_device, imageSemaphore, nullptr);

  for (const auto &renderSemaphore : m_renderSemaphores)
    if (renderSemaphore != nullptr)
      vkDestroySemaphore(m_device, renderSemaphore, nullptr);

  for (const auto &inFlightFence : m_inFlightFences)
    if (inFlightFence != nullptr)
      vkDestroyFence(m_device, inFlightFence, nullptr);

  if (m_device != nullptr)
    vkDestroyDevice(m_device, nullptr);

  if (m_surfaceKHR != nullptr)
    vkDestroySurfaceKHR(m_instance, m_surfaceKHR, nullptr);

  auto vkDestroyDebugUtilsMessengerEXT{reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"))};
  if (vkDestroyDebugUtilsMessengerEXT != nullptr && m_debugMessenger != nullptr)
    vkDestroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, nullptr);

  if (m_instance != nullptr)
    vkDestroyInstance(m_instance, nullptr);
}
