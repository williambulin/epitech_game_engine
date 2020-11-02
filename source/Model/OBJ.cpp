#include "OBJ.hpp"

#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../Maths/Vectors.hpp"

static const std::vector<std::string> supportedIndexes = {"v", "vn", "vt", "f"};

OBJ::OBJ(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open())
    throw std::runtime_error(path + ": Invalid file path.");
  std::string                           lineString;
  Vertices                              vertices;
  Normals                               normals;
  Texcoords                             texcoords;
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
      data.push_back(dataPart);

    // fill vectors with the data
    if (index == "f") {
      faces.push_back(data);
    } else {
      std::vector<float> dataFloat;
      for (std::string &dataPart : data)
        dataFloat.push_back(std::stof(dataPart));
      if (index == "v" && dataFloat.size() == 3)
        vertices.push_back(Vector3<float>(dataFloat));
      else if (index == "vn" && dataFloat.size() == 3)
        normals.push_back(Vector3<float>(dataFloat));
      else if (index == "vt" && dataFloat.size() == 2)
        texcoords.push_back(Vector2<float>(dataFloat));
    }
  }
  file.close();

  // sort the data based on face indexes
  for (std::vector<std::string> &face : faces) {
    for (std::string &faceElement : face) {
      auto start = 0U;
      auto end   = faceElement.find("/");
      for (auto i = 0U; i < 3 && end != std::string::npos; i++) {
        auto faceIndex = std::stoi(faceElement.substr(start, end - start)) - 1;
        if (i == 0)
          m_vertices.push_back(vertices[faceIndex]);
        else if (i == 1)
          m_texcoords.push_back(texcoords[faceIndex]);
        else
          m_normals.push_back(normals[faceIndex]);
        start = end + 1;
        end   = faceElement.find("/");
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

auto OBJ::getNormals() const -> const Normals & {
  return m_normals;
}

auto OBJ::getTexcoords() const -> const Texcoords & {
  return m_texcoords;
}