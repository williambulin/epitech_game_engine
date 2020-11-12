#include "Model.hpp"
#include "Model/OBJ.hpp"

Vulkan::Model::Model(VkPhysicalDevice physicalDevice, VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue, const std::string &path) {
  constexpr bool useOBJ{true};
  if constexpr (useOBJ) {
    OBJ obj{path};

    std::unordered_map<Vertex, std::uint32_t> uniqueVertices{};

    for (std::size_t i{0}; i < obj.getVertices().size(); ++i) {
      // if (index.texcoord_index == -1)
      //   break;

      Vertex vertex{
      .position        = obj.getVertices()[i],
      .color           = ml::vec3{1.0f, 1.0f, 1.0f},
      .texturePosition = obj.getTexcoords()[i],
      .normal          = obj.getNormals()[i],
      };

      if (!uniqueVertices.contains(vertex)) {
        uniqueVertices[vertex] = static_cast<std::uint32_t>(m_vertices.size());
        m_vertices.push_back(vertex);
      }

      m_indicesList.push_back(uniqueVertices[vertex]);
    }
  } else {
    // tinyobj::attrib_t                attrib{};
    // std::vector<tinyobj::shape_t>    shapes{};
    // std::vector<tinyobj::material_t> materials{};
    // std::string                      error{};

    // if (!tinyobj::LoadObj(std::addressof(attrib), std::addressof(shapes), std::addressof(materials), std::addressof(error), path.c_str()))
    //   throw std::runtime_error{error};

    // std::unordered_map<Vertex, std::uint32_t> uniqueVertices{};

    // for (const auto &shape : shapes) {
    //   for (const auto &index : shape.mesh.indices) {
    //     if (index.texcoord_index == -1)
    //       break;

    //     Vertex vertex{
    //     .position =
    //     ml::vec3{
    //     attrib.vertices[static_cast<std::size_t>(3 * index.vertex_index + 0)],
    //     attrib.vertices[static_cast<std::size_t>(3 * index.vertex_index + 1)],
    //     attrib.vertices[static_cast<std::size_t>(3 * index.vertex_index + 2)],
    //     },
    //     .color = ml::vec3{1.0f, 1.0f, 1.0f},
    //     .texturePosition =
    //     ml::vec2{
    //     attrib.texcoords[static_cast<std::size_t>(2 * index.texcoord_index + 0)],
    //     1.0f - attrib.texcoords[static_cast<std::size_t>(2 * index.texcoord_index + 1)],
    //     },
    //     };

    //     if (!uniqueVertices.contains(vertex)) {
    //       uniqueVertices[vertex] = static_cast<std::uint32_t>(m_vertices.size());
    //       m_vertices.push_back(vertex);
    //     }

    //     m_indicesList.push_back(uniqueVertices[vertex]);
    //   }
    // }
  }

  std::size_t bufferSize{sizeof(Vertex) * m_vertices.size()};
  if (bufferSize <= 0)
    throw std::runtime_error{"Couldn't load model (no vertices/data to upload to the queue)"};
  using (Buffer vertexStagingBuffer{physicalDevice, device, commandPool, graphicsQueue, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}) {
    vertexStagingBuffer.cpy(m_vertices.data(), bufferSize);
    m_vertexBuffer = std::make_unique<Buffer>(physicalDevice, device, commandPool, graphicsQueue, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_vertexBuffer->buffercpy(vertexStagingBuffer.m_buffer, bufferSize);
  }

  bufferSize = sizeof(std::uint32_t) * m_indicesList.size();
  using (Buffer indicesStagingBuffer{physicalDevice, device, commandPool, graphicsQueue, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT}) {
    indicesStagingBuffer.cpy(m_indicesList.data(), bufferSize);
    m_indicesBuffer = std::make_unique<Buffer>(physicalDevice, device, commandPool, graphicsQueue, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    m_indicesBuffer->buffercpy(indicesStagingBuffer.m_buffer, bufferSize);
  }
}
