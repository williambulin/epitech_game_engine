#include "Model.hpp"
#include "Model/OBJ.hpp"

Vulkan::Model::Model(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, const std::string &path, const ml::vec3 &color) {
  OBJ obj{path};

  std::unordered_map<Vertex, std::uint32_t> uniqueVertices{};

  for (std::size_t i{0}; i < obj.getVertices().size(); ++i) {
    // if (index.texcoord_index == -1)
    //   break;

    Vertex vertex{
    .position        = obj.getVertices()[i],
    .color           = color,
    .texturePosition = obj.getTexcoords()[i],
    .normal          = obj.getNormals()[i],
    };

    if (!uniqueVertices.contains(vertex)) {
      uniqueVertices[vertex] = static_cast<std::uint32_t>(m_vertices.size());
      m_vertices.push_back(vertex);
    }

    m_indicesList.push_back(uniqueVertices[vertex]);
  }

  std::size_t bufferSize{sizeof(Vertex) * m_vertices.size()};
  if (bufferSize <= 0)
    throw std::runtime_error{"Couldn't load model (no vertices/data to upload to the queue)"};
  use(Buffer vertexStagingBuffer{physicalDevice, device, commandPool, graphicsQueue, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}) {
    vertexStagingBuffer.cpy(m_vertices.data(), bufferSize);
    m_vertexBuffer = std::make_unique<Buffer>(physicalDevice, device, commandPool, graphicsQueue, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_vertexBuffer->buffercpy(vertexStagingBuffer.m_buffer, bufferSize);
  }

  bufferSize = sizeof(std::uint32_t) * m_indicesList.size();
  use(Buffer indicesStagingBuffer{physicalDevice, device, commandPool, graphicsQueue, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}) {
    indicesStagingBuffer.cpy(m_indicesList.data(), bufferSize);
    m_indicesBuffer = std::make_unique<Buffer>(physicalDevice, device, commandPool, graphicsQueue, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_indicesBuffer->buffercpy(indicesStagingBuffer.m_buffer, bufferSize);
  }
}
