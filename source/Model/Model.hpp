#pragma once

#include <vector>

#include "../Maths/Vec2.hpp"
#include "../Maths/Vec3.hpp"
#include "IModel.hpp"

class Model : public IModel {
protected:
  Vertices  m_vertices{};
  Normals   m_normals{};
  Texcoords m_texcoords{};

public:
  [[nodiscard]] auto getVertices() const -> const Vertices &;
  [[nodiscard]] auto getNormals() const -> const Normals &;
  [[nodiscard]] auto getTexcoords() const -> const Texcoords &;
};