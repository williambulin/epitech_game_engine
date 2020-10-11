#include <exception>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../Maths/Vec2.hpp"
#include "../Maths/Vec3.hpp"
#include "OBJ.hpp"

static const std::vector<std::string> supportedIndexes = {"v", "vn", "vt", "f"};

OBJ::OBJ(const std::string &path) {
  std::ifstream file(path);
  std::string   lineString;
  Vertices      vertices;
  Normals       normals;
  Texcoords     texcoords;
  std::vector<std::vector<std::string>> faces;

  if (!file.is_open())
    throw std::runtime_error(path + ": Invalid file path.");
  while (std::getline(file, lineString)) {
    if (lineString.length() > 0) {

      //extract the data from the file
      std::istringstream line(lineString);
      std::string        index;
      line >> index;
      if (std::find(supportedIndexes.begin(), supportedIndexes.end(), index) == supportedIndexes.end())
        continue;
      std::vector<std::string> data;
      for (std::string dataPart; line >> dataPart;)
        data.push_back(dataPart);

      //fill vectors with the data
      if (index == "f") {
        faces.push_back(data);
      } else {
        std::vector<float> dataFloat;
        for (std::string &dataPart : data)
          dataFloat.push_back(std::stof(dataPart));
        if (index == "v" && dataFloat.size() == 3)
          vertices.push_back(Vec3(dataFloat));
        else if (index == "vn" && dataFloat.size() == 3)
          normals.push_back(Vec3(dataFloat));
        else if (index == "vt" && dataFloat.size() == 2)
          texcoords.push_back(Vec2(dataFloat));
      }
    }
  }
  file.close();

  //sort the data based on face indexes
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