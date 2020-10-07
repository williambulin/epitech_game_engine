#pragma once

#include <vector>

#include "../Maths/Vec2.hpp"
#include "../Maths/Vec3.hpp"

class IModel {
public:
  using Vertices  = std::vector<Vec3>;
  using Normals   = std::vector<Vec3>;
  using Texcoords = std::vector<Vec2>;

public:
  virtual ~IModel() = default;

  [[nodiscard]] virtual const Vertices &getVertices(void) const	  = 0;
  [[nodiscard]] virtual const Normals &getNormals(void) const	  = 0;
  [[nodiscard]] virtual const Texcoords &getTexcoords(void) const = 0;
};