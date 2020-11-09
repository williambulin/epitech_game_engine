#pragma once

#include <vector>

#include "Extension/Libraries/glm.hpp"
#include "Extension/Libraries/Vulkan.hpp"

class Vertex {
public:
  glm::vec3 position{};
  glm::vec3 color{};
  glm::vec2 texturePosition{};

  [[nodiscard]] static auto bindingDescription() -> VkVertexInputBindingDescription;
  [[nodiscard]] static auto attributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>;

  [[nodiscard]] bool operator==(const Vertex &vertex) const;
};

namespace std {
  template <>
  struct hash<Vertex> {
    size_t operator()(Vertex const &vertex) const {
      return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texturePosition) << 1);
    }
  };
}
