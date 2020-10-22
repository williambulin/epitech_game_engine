#pragma once

#include <assimp/mesh.h>
#include <string>
#include <vector>

#include "IModel.hpp"

class Model : public IModel {
protected:
  Vertices  m_vertices{};
  Normals   m_normals{};
  Texcoords m_texcoords{};

public:
  explicit Model() {}  // Does nothing to avoid calling it in the child class and parsing the file twice
  explicit Model(const std::string &);
  Model(const Model &);
  ~Model() = default;

  [[nodiscard]] auto getVertices() const -> const Vertices &;
  [[nodiscard]] auto getNormals() const -> const Normals &;
  [[nodiscard]] auto getTexcoords() const -> const Texcoords &;

protected:
  void loadMesh(const aiMesh *);
};