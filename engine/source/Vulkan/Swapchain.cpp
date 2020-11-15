#include "Swapchain.hpp"
#include "UniformBufferData.hpp"

#include <string>

auto Vulkan::Swapchain::getExtend2D() const noexcept -> const VkExtent2D & {
  return m_extent2D;
}

auto Vulkan::Swapchain::getFormat() const noexcept -> const VkFormat & {
  return m_format;
}

auto Vulkan::Swapchain::findSupportedFormat(const std::vector<VkFormat> &formats, VkImageTiling imageTiling, VkFormatFeatureFlags formatFeatureFlags) const -> VkFormat {
  for (auto format : formats) {
    VkFormatProperties formatProperties{};
    vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, std::addressof(formatProperties));

    if (imageTiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & formatFeatureFlags))
      return format;
    else if (imageTiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & formatFeatureFlags))
      return format;
  }

  throw std::runtime_error{"Couldn't find suitable supported format"};
}

auto Vulkan::Swapchain::findDepthFormat() const -> VkFormat {
  return findSupportedFormat(
  {
  VK_FORMAT_D32_SFLOAT,
  VK_FORMAT_D32_SFLOAT_S8_UINT,
  VK_FORMAT_D24_UNORM_S8_UINT,
  },
  VK_IMAGE_TILING_OPTIMAL,
  VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

auto Vulkan::Swapchain::loadShader(const std::string &path) const -> std::vector<char> {
  std::ifstream fileStream{path, std::ios::ate | std::ios::binary};
  if (!fileStream.is_open())
    throw std::runtime_error{"Couldn't read file " + path};

  std::vector<char> shaderBytes(static_cast<std::size_t>(fileStream.tellg()));
  fileStream.seekg(0);
  fileStream.read(shaderBytes.data(), static_cast<std::int64_t>(shaderBytes.size()));
  fileStream.close();
  return shaderBytes;
}

auto Vulkan::Swapchain::createShader(const std::vector<char> &bytes) const -> VkShaderModule {
  VkShaderModuleCreateInfo shaderModuleCreateInfo{
  .sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
  .codeSize = bytes.size(),
  .pCode    = reinterpret_cast<const std::uint32_t *>(bytes.data()),
  };

  VkShaderModule shaderModule{nullptr};
  if (vkCreateShaderModule(m_device, std::addressof(shaderModuleCreateInfo), nullptr, std::addressof(shaderModule)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create shader module"};
  return shaderModule;
}

Vulkan::Swapchain::Swapchain(VkCommandPool commandPool, VkQueue graphicsQueue, HWND hwnd, const VkPhysicalDevice physicalDevice, const VkDevice device, const VkSurfaceKHR surfaceKHR, const VkDescriptorSetLayout descriptorSetLayout, const QueueFamilies &queueFamilies) : m_physicalDevice{physicalDevice}, m_device{device}, m_commandPool{commandPool}, m_graphicsQueue{graphicsQueue} {
  vkDeviceWaitIdle(m_device);

  // Creating the Swap Chain
  SwapchainDetails swapchainDetails{m_physicalDevice, surfaceKHR};
  const auto &     surfaceFormat{swapchainDetails.getBestFormat()};
  auto             presentMode{swapchainDetails.getBestPresentMode()};
  std::uint32_t    imageCount{std::clamp(swapchainDetails.m_capabilities.minImageCount + 1, swapchainDetails.m_capabilities.minImageCount, swapchainDetails.m_capabilities.maxImageCount)};

  m_extent2D = swapchainDetails.getSwapExtent(hwnd, 1600, 900);
  m_format   = surfaceFormat.format;

  VkSwapchainCreateInfoKHR swapChainCreateInfo{
  .sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
  .surface          = surfaceKHR,
  .minImageCount    = imageCount,
  .imageFormat      = surfaceFormat.format,
  .imageColorSpace  = surfaceFormat.colorSpace,
  .imageExtent      = m_extent2D,
  .imageArrayLayers = 1,
  .imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
  .preTransform     = swapchainDetails.m_capabilities.currentTransform,
  .compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
  .presentMode      = presentMode,
  .clipped          = true,
  .oldSwapchain     = nullptr,
  };

  std::vector<std::uint32_t> indices{queueFamilies.m_presentQueue.value(), queueFamilies.m_graphicsQueue.value()};
  if (std::ranges::all_of(indices, [&indices](auto index) {
        return index == indices.front();
      }))
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  else {
    swapChainCreateInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    swapChainCreateInfo.queueFamilyIndexCount = static_cast<std::uint32_t>(indices.size());
    swapChainCreateInfo.pQueueFamilyIndices   = indices.data();
  }

  if (vkCreateSwapchainKHR(m_device, std::addressof(swapChainCreateInfo), nullptr, std::addressof(m_swapchain)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create swap chain"};

  std::uint32_t swapchainImageCount{0};
  vkGetSwapchainImagesKHR(m_device, m_swapchain, std::addressof(swapchainImageCount), nullptr);
  std::vector<VkImage> swapchainImages(swapchainImageCount);
  vkGetSwapchainImagesKHR(m_device, m_swapchain, std::addressof(swapchainImageCount), swapchainImages.data());

  // Image and Image Views
  for (auto swapchainImage : swapchainImages)
    m_images.emplace_back(std::make_unique<Image>(m_device, m_physicalDevice, m_commandPool, m_graphicsQueue, swapchainImage, *this));

  // Render Pass
  VkAttachmentDescription attachmentDescriptionColor{
  .format         = m_format,
  .samples        = VK_SAMPLE_COUNT_1_BIT,
  .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
  .storeOp        = VK_ATTACHMENT_STORE_OP_STORE,
  .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
  .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
  .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
  .finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
  };

  VkAttachmentDescription attachmentDescriptionDepth{
  .format         = findDepthFormat(),
  .samples        = VK_SAMPLE_COUNT_1_BIT,
  .loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR,
  .storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE,
  .stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
  .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
  .initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
  .finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  std::vector<VkAttachmentDescription> attachmentDescriptions{attachmentDescriptionColor, attachmentDescriptionDepth};

  VkAttachmentReference attachmentReferenceColor{
  .attachment = 0,
  .layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  };

  VkAttachmentReference attachmentReferenceDepth{
  .attachment = 1,
  .layout     = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  };

  VkSubpassDescription subpassDescription{
  .pipelineBindPoint       = VK_PIPELINE_BIND_POINT_GRAPHICS,
  .colorAttachmentCount    = 1,
  .pColorAttachments       = std::addressof(attachmentReferenceColor),
  .pDepthStencilAttachment = std::addressof(attachmentReferenceDepth),
  };

  VkSubpassDependency subpassDependency{
  .srcSubpass    = VK_SUBPASS_EXTERNAL,
  .dstSubpass    = 0,
  .srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
  .dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
  .srcAccessMask = 0,
  .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
  };

  VkRenderPassCreateInfo renderPassCreateInfo{
  .sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
  .attachmentCount = static_cast<std::uint32_t>(attachmentDescriptions.size()),
  .pAttachments    = attachmentDescriptions.data(),
  .subpassCount    = 1,
  .pSubpasses      = std::addressof(subpassDescription),
  .dependencyCount = 1,
  .pDependencies   = std::addressof(subpassDependency),
  };

  if (vkCreateRenderPass(m_device, std::addressof(renderPassCreateInfo), nullptr, std::addressof(m_renderPass)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create the render pass"};

  // Shaders and Pipeline
  auto triangleVertexShaderBytes{loadShader("../shaders/triangle.vert.spv")};
  auto triangleFragmentShaderBytes{loadShader("../shaders/triangle.frag.spv")};
  auto triangleVertexShader{createShader(triangleVertexShaderBytes)};
  auto triangleFragmentShader{createShader(triangleFragmentShaderBytes)};

  VkPipelineShaderStageCreateInfo vertexPipelineShaderStageCreateInfo{
  .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
  .stage  = VK_SHADER_STAGE_VERTEX_BIT,
  .module = triangleVertexShader,
  .pName  = "main",
  };

  VkPipelineShaderStageCreateInfo fragmentPipelineShaderStageCreateInfo{
  .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
  .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
  .module = triangleFragmentShader,
  .pName  = "main",
  };

  std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos{vertexPipelineShaderStageCreateInfo, fragmentPipelineShaderStageCreateInfo};

  auto vertexBindingDescription{Vertex::bindingDescription()};
  auto vertexAttributeDescriptions{Vertex::attributeDescriptions()};

  VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo{
  .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
  .vertexBindingDescriptionCount   = 1,
  .pVertexBindingDescriptions      = std::addressof(vertexBindingDescription),
  .vertexAttributeDescriptionCount = static_cast<std::uint32_t>(vertexAttributeDescriptions.size()),
  .pVertexAttributeDescriptions    = vertexAttributeDescriptions.data(),
  };

  VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo{
  .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
  .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
  .primitiveRestartEnable = VK_FALSE,
  };

  VkViewport viewport{
  .x        = 0.0f,
  .y        = 0.0f,
  .width    = static_cast<float>(m_extent2D.width),
  .height   = static_cast<float>(m_extent2D.height),
  .minDepth = 0.0f,
  .maxDepth = 1.0f,
  };

  VkRect2D scissorRect{
  .offset = {0, 0},
  .extent = m_extent2D,
  };

  VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo{
  .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
  .viewportCount = 1,
  .pViewports    = std::addressof(viewport),
  .scissorCount  = 1,
  .pScissors     = std::addressof(scissorRect),
  };

  VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo{
  .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
  .depthClampEnable        = VK_FALSE,
  .rasterizerDiscardEnable = VK_FALSE,
  .polygonMode             = VK_POLYGON_MODE_FILL,
  .cullMode                = VK_CULL_MODE_NONE,                // VK_CULL_MODE_NONE or VK_CULL_MODE_BACK_BIT,
  .frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE,  // VK_FRONT_FACE_CLOCKWISE,
  .depthBiasEnable         = VK_FALSE,
  .lineWidth               = 1.0f,
  };

  VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo{
  .sType                = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
  .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
  .sampleShadingEnable  = VK_FALSE,
  };

  VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState{
  .blendEnable    = VK_FALSE,
  .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
  };

  VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo{
  .sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
  .logicOpEnable   = VK_FALSE,
  .attachmentCount = 1,
  .pAttachments    = std::addressof(pipelineColorBlendAttachmentState),
  };

  VkPushConstantRange pushConstantRange{
  .stageFlags = VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
  .offset     = 0,
  .size       = sizeof(float),
  };

  VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{
  .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
  .setLayoutCount         = 1,
  .pSetLayouts            = std::addressof(descriptorSetLayout),
  .pushConstantRangeCount = 1,
  .pPushConstantRanges    = std::addressof(pushConstantRange),
  };

  if (vkCreatePipelineLayout(m_device, std::addressof(pipelineLayoutCreateInfo), nullptr, std::addressof(m_pipelineLayout)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create pipeline layout"};

  VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo{
  .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
  .depthTestEnable       = VK_TRUE,
  .depthWriteEnable      = VK_TRUE,
  .depthCompareOp        = VK_COMPARE_OP_LESS,
  .depthBoundsTestEnable = VK_FALSE,
  .stencilTestEnable     = VK_FALSE,
  };

  VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{
  .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
  .stageCount          = static_cast<std::uint32_t>(pipelineShaderStageCreateInfos.size()),
  .pStages             = pipelineShaderStageCreateInfos.data(),
  .pVertexInputState   = std::addressof(pipelineVertexInputStateCreateInfo),
  .pInputAssemblyState = std::addressof(pipelineInputAssemblyStateCreateInfo),
  .pViewportState      = std::addressof(pipelineViewportStateCreateInfo),
  .pRasterizationState = std::addressof(pipelineRasterizationStateCreateInfo),
  .pMultisampleState   = std::addressof(pipelineMultisampleStateCreateInfo),
  .pDepthStencilState  = std::addressof(pipelineDepthStencilStateCreateInfo),
  .pColorBlendState    = std::addressof(pipelineColorBlendStateCreateInfo),
  .layout              = m_pipelineLayout,
  .renderPass          = m_renderPass,
  .subpass             = 0,
  };

  if (vkCreateGraphicsPipelines(m_device, nullptr, 1, std::addressof(graphicsPipelineCreateInfo), nullptr, std::addressof(m_graphicsPipeline)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create graphics pipeline"};

  vkDestroyShaderModule(m_device, triangleVertexShader, nullptr);
  vkDestroyShaderModule(m_device, triangleFragmentShader, nullptr);

  // Depth images
  auto depthFormat{findDepthFormat()};
  m_depthImage = std::make_unique<Image>(m_device, m_physicalDevice, m_commandPool, m_graphicsQueue, m_extent2D.width, m_extent2D.height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT);
  m_depthImage->transitionLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

  // Framebuffer images
  m_framebuffers.resize(m_images.size(), nullptr);
  for (auto it{m_images.begin()}; it != m_images.end(); ++it) {
    auto index{std::distance(m_images.begin(), it)};

    std::vector<VkImageView> attachments{(*it)->m_imageView, m_depthImage->m_imageView};

    VkFramebufferCreateInfo framebufferCreateInfo{
    .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
    .renderPass      = m_renderPass,
    .attachmentCount = static_cast<std::uint32_t>(attachments.size()),
    .pAttachments    = attachments.data(),
    .width           = m_extent2D.width,
    .height          = m_extent2D.height,
    .layers          = 1,
    };

    if (vkCreateFramebuffer(m_device, std::addressof(framebufferCreateInfo), nullptr, std::addressof(m_framebuffers[static_cast<std::size_t>(index)])) != VK_SUCCESS)
      throw std::runtime_error{"Couldn't create the swapchain's framebuffer " + std::to_string(index)};
  }

  // Limits
  constexpr std::uint32_t descriptorSetCount{256};

  // Uniform buffer
  for (std::size_t i{0}; i < descriptorSetCount; ++i)
    m_uniformBuffers.emplace_back(std::make_unique<Buffer>(physicalDevice, device, commandPool, graphicsQueue, sizeof(UniformBufferData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT));

  // Descriptor pool
  VkDescriptorPoolSize descriptorPoolSizeUBO{
  .type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
  .descriptorCount = descriptorSetCount,  // static_cast<std::uint32_t>(m_images.size()),
  };

  VkDescriptorPoolSize descriptorPoolSizeTextureSampler{
  .type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
  .descriptorCount = descriptorSetCount,  // static_cast<std::uint32_t>(m_images.size()),
  };

  std::vector<VkDescriptorPoolSize> descriptorPoolSizes{descriptorPoolSizeUBO, descriptorPoolSizeTextureSampler};

  VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{
  .sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
  .maxSets       = descriptorSetCount,  // static_cast<std::uint32_t>(m_images.size()),
  .poolSizeCount = static_cast<std::uint32_t>(descriptorPoolSizes.size()),
  .pPoolSizes    = descriptorPoolSizes.data(),
  };

  if (vkCreateDescriptorPool(m_device, std::addressof(descriptorPoolCreateInfo), nullptr, std::addressof(m_descriptorPool)) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't create descriptor pool"};

  // Descriptor sets
  std::vector<VkDescriptorSetLayout> descriptorSetLayouts(descriptorSetCount, descriptorSetLayout);

  VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{
  .sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
  .descriptorPool     = m_descriptorPool,
  .descriptorSetCount = descriptorSetCount,  // static_cast<std::uint32_t>(m_images.size()),
  .pSetLayouts        = descriptorSetLayouts.data(),
  };

  m_descriptorSets.resize(descriptorSetCount);
  if (vkAllocateDescriptorSets(m_device, std::addressof(descriptorSetAllocateInfo), m_descriptorSets.data()) != VK_SUCCESS)
    throw std::runtime_error{"Couldn't allocate descriptor sets"};

  // Swapchain Image fences
  m_imageFences.resize(m_images.size(), nullptr);
}

Vulkan::Swapchain::~Swapchain() {
  vkDeviceWaitIdle(m_device);

  for (auto &uniformBuffer : m_uniformBuffers)
    uniformBuffer.reset();

  if (m_descriptorPool != nullptr)
    vkDestroyDescriptorPool(m_device, m_descriptorPool, nullptr);

  for (auto &uniformBuffer : m_uniformBuffers)
    uniformBuffer.reset();

  for (auto framebuffer : m_framebuffers)
    if (framebuffer != nullptr)
      vkDestroyFramebuffer(m_device, framebuffer, nullptr);

  m_depthImage.reset();

  if (m_graphicsPipeline != nullptr)
    vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);

  if (m_pipelineLayout != nullptr)
    vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);

  if (m_renderPass != nullptr)
    vkDestroyRenderPass(m_device, m_renderPass, nullptr);

  for (auto &image : m_images)
    image.reset();

  if (m_swapchain != nullptr)
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
}
