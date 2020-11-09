#pragma once

#include "Extension/Libraries/tiny_obj_loader.h"
#include "Extension/Language/Keywords.hpp"
#include "Vertex.hpp"
#include "Buffer.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <stdexcept>

namespace Vulkan {
  class Model;
}

class Vulkan::Model final {
public:
  std::unique_ptr<Buffer>    m_vertexBuffer{nullptr};
  std::unique_ptr<Buffer>    m_indicesBuffer{nullptr};
  std::vector<Vertex>        m_vertices{};
  std::vector<std::uint32_t> m_indicesList{};

public:
  explicit Model(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, const std::string &path);
};
