#include "Model.hpp"

auto Model::getVertices() const -> const Vertices & {
  return m_vertices;
}

auto Model::getNormals() const -> const Normals & {
  return m_normals;
}

auto Model::getTexcoords() const -> const Texcoords & {
  return m_texcoords;
}