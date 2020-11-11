#pragma once

#include <string>
#include <vector>

#include "IModel.hpp"

class OBJ final : public IModel {
private:
  Vertices   m_vertices{};
  Texcoords  m_texcoords{};
  Normals    m_normals{};
  Tangents   m_tangents{};
  Bitangents m_bitangents{};

public:
  explicit OBJ(const std::string &);
  OBJ(const OBJ &);
  ~OBJ() = default;

  [[nodiscard]] auto getVertices() const -> const Vertices &;
  [[nodiscard]] auto getTexcoords() const -> const Texcoords &;
  [[nodiscard]] auto getNormals() const -> const Normals &;
  [[nodiscard]] auto getTangents() const -> const Tangents &;
  [[nodiscard]] auto getBitangents() const -> const Bitangents &;
};