#pragma once

#include <vector>

#include "../Maths/Vectors.hpp"

class IModel {
public:
  using Vertices   = std::vector<Vector3<float>>;
  using Texcoords  = std::vector<Vector2<float>>;
  using Normals    = std::vector<Vector3<float>>;
  using Tangents   = std::vector<Vector3<float>>;
  using Bitangents = std::vector<Vector3<float>>;

public:
  virtual ~IModel() = default;

  [[nodiscard]] virtual const Vertices &  getVertices() const   = 0;
  [[nodiscard]] virtual const Texcoords & getTexcoords() const  = 0;
  [[nodiscard]] virtual const Normals &   getNormals() const    = 0;
  [[nodiscard]] virtual const Tangents &  getTangents() const   = 0;
  [[nodiscard]] virtual const Bitangents &getBitangents() const = 0;
};