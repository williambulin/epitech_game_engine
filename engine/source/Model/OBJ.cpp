#include "OBJ.hpp"

#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../Maths/Vectors.hpp"

#include <iostream>

static const std::vector<std::string> supportedIndexes = {"v", "vn", "vt", "f"};

OBJ::OBJ(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error(path + ": Invalid file path.");
  std::string                           lineString;
  Vertices                              vertices;
  Texcoords                             texcoords;
  Normals                               normals;
  std::vector<std::vector<std::string>> faces;

  while (std::getline(file, lineString)) {
    // extract the data from the file
    if (lineString.length() == 0)
      continue;
    std::istringstream line(lineString);
    std::string        index;
    line >> index;
    if (std::find(supportedIndexes.begin(), supportedIndexes.end(), index) == supportedIndexes.end())
      continue;
    std::vector<std::string> data;
    for (std::string dataPart; line >> dataPart;)
      if (!dataPart.empty())
        data.push_back(dataPart);

    // fill vectors with the data
    if (index == "f") {
      if (data.size() == 3) {
        faces.push_back(data);
      } else {
        throw std::runtime_error(path + ": Model uses QUADS which are not supported.");
      }
    } else {
      std::vector<float> dataFloat;
      for (std::string &dataPart : data)
        dataFloat.push_back(std::stof(dataPart));
      if (index == "v" && dataFloat.size() == 3) {
        vertices.push_back(Vector3<float>{dataFloat});
      } else if (index == "vt" && (dataFloat.size() == 2 || dataFloat.size() == 3)) {
        texcoords.push_back(Vector2<float>{dataFloat[0], 1.0f - dataFloat[1]});
      } else if (index == "vn" && dataFloat.size() == 3) {
        normals.push_back(Vector3<float>{dataFloat});
      }
    }
  }
  file.close();

  // sort the data based on face indexes
  for (std::vector<std::string> &face : faces) {
    for (std::string &faceElement : face) {
      std::istringstream faceStream(faceElement);
      std::string        faceIndexStr;
      for (auto i = 0U; std::getline(faceStream, faceIndexStr, '/');) {
        if (faceIndexStr.empty())
          continue;
        auto faceIndex = std::stoi(faceIndexStr) - 1;
        if (i == 0) {
          m_vertices.push_back(vertices[faceIndex]);
        } else if (i == 1) {
          m_texcoords.push_back(texcoords[faceIndex]);
        } else if (i == 2) {
          m_normals.push_back(normals[faceIndex]);
        }
        i++;
      }
    }
  }

  bool computeNormals = m_normals.size() < m_vertices.size();
  if (computeNormals)
    m_normals.clear();
  bool computeTangents = m_vertices.size() == m_texcoords.size();
  for (auto i = 0U; i < m_vertices.size(); i += 3) {
    auto &v0 = m_vertices[i];
    auto &v1 = m_vertices[i + 1];
    auto &v2 = m_vertices[i + 2];

    // if some normals are missing, compute them
    if (computeNormals) {
      Vector3<float> a      = v1 - v0;
      auto           b      = v2 - v0;
      auto           normal = a.cross(b);
      normal.normalize();
      for (auto j = 0; j < 3; j++)
        m_normals.push_back(normal);
    }

    // compute tangents and bitangents
    if (computeTangents) {
      auto &uv0       = m_texcoords[i];
      auto &uv1       = m_texcoords[i + 1];
      auto &uv2       = m_texcoords[i + 2];
      auto  deltaPos1 = v1 - v0;
      auto  deltaPos2 = v2 - v0;
      auto  deltaUv1  = uv1 - uv0;
      auto  deltaUv2  = uv2 - uv0;
      float r         = 1.0f / (deltaUv1[0] * deltaUv2[1] - deltaUv1[1] * deltaUv2[0]);
      auto  tangent   = (deltaPos1 * deltaUv2[1] - deltaPos2 * deltaUv1[1]) * r;
      auto  bitangent = (deltaPos2 * deltaUv1[0] - deltaPos1 * deltaUv2[0]) * r;
      for (auto j = 0U; j < 3; j++) {
        m_tangents.push_back(tangent);
        m_bitangents.push_back(bitangent);
      }
    }
  }
}

OBJ::OBJ(const OBJ &copy) {
  m_vertices  = copy.getVertices();
  m_normals   = copy.getNormals();
  m_texcoords = copy.getTexcoords();
}

auto OBJ::getVertices() const -> const Vertices & {
  return m_vertices;
}

auto OBJ::getTexcoords() const -> const Texcoords & {
  return m_texcoords;
}

auto OBJ::getNormals() const -> const Normals & {
  return m_normals;
}

auto OBJ::getTangents() const -> const Tangents & {
  return m_tangents;
}

auto OBJ::getBitangents() const -> const Bitangents & {
  return m_bitangents;
}