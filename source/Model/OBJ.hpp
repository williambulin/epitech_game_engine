#pragma once

#include <string>
#include <vector>

#include "IModel.hpp"

class OBJ : public IModel {
protected:
  Vertices  m_vertices{};
  Normals   m_normals{};
  Texcoords m_texcoords{};

public:
  explicit OBJ(const std::string &);
  OBJ(const OBJ &);
  ~OBJ() = default;

  [[nodiscard]] auto getVertices() const -> const Vertices &;
  [[nodiscard]] auto getNormals() const -> const Normals &;
  [[nodiscard]] auto getTexcoords() const -> const Texcoords &;
};