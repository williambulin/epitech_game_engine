#pragma once

#include <vector>

#include "../Maths/Vectors.hpp"

class IModel {
public:
  using Vertices  = std::vector<Vector<float, 3U>>;
  using Normals   = std::vector<Vector<float, 3U>>;
  using Texcoords = std::vector<Vector<float, 2U>>;

public:
  virtual ~IModel() = default;

  [[nodiscard]] virtual const Vertices &getVertices(void) const	  = 0;
  [[nodiscard]] virtual const Normals &getNormals(void) const	  = 0;
  [[nodiscard]] virtual const Texcoords &getTexcoords(void) const = 0;
};