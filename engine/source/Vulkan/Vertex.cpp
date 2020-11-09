#include "Vertex.hpp"

auto Vertex::bindingDescription() -> VkVertexInputBindingDescription {
  return VkVertexInputBindingDescription{
  .binding   = 0,
  .stride    = sizeof(Vertex),
  .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
  };
}

auto Vertex::attributeDescriptions() -> std::vector<VkVertexInputAttributeDescription> {
  return std::vector<VkVertexInputAttributeDescription>{
  VkVertexInputAttributeDescription{
  .location = 0,
  .binding  = 0,
  .format   = VK_FORMAT_R32G32B32_SFLOAT,
  .offset   = offsetof(Vertex, position),
  },
  VkVertexInputAttributeDescription{
  .location = 1,
  .binding  = 0,
  .format   = VK_FORMAT_R32G32B32_SFLOAT,
  .offset   = offsetof(Vertex, color),
  },
  VkVertexInputAttributeDescription{
  .location = 2,
  .binding  = 0,
  .format   = VK_FORMAT_R32G32_SFLOAT,
  .offset   = offsetof(Vertex, texturePosition),
  },
  };
}

bool Vertex::operator==(const Vertex &vertex) const {
  return position == vertex.position && color == vertex.color && texturePosition == vertex.texturePosition;
}
