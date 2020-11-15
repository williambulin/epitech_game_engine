#pragma once

#include <vector>

#include "Maths/Math.hpp"
#include "Extension/Libraries/Vulkan.hpp"

class Vertex {
public:
  ml::vec3 position{0.0f, 0.0f, 0.0f};
  ml::vec3 color{0.0f, 0.0f, 0.0f};
  ml::vec2 texturePosition{0.0f, 0.0f};
  ml::vec3 normal{0.0f, 0.0f, 0.0f};

  [[nodiscard]] static auto bindingDescription() -> VkVertexInputBindingDescription;
  [[nodiscard]] static auto attributeDescriptions() -> std::vector<VkVertexInputAttributeDescription>;

  [[nodiscard]] bool operator==(const Vertex &vertex) const;
};

namespace std {
  template <>
  struct hash<Vertex> {
    size_t operator()(Vertex const &vertex) const {
      return ((vertex.position.hash() ^ (vertex.color.hash() << 1)) >> 1) ^ (vertex.texturePosition.hash() << 1);
    }
  };
}
