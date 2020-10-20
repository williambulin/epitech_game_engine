#include "Model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <string>
#include <vector>

#include "../Maths/Vectors.hpp"
#include "Converter/Assimp.hpp"

static const unsigned int modelProcessing = aiPostProcessSteps::aiProcess_CalcTangentSpace | aiPostProcessSteps::aiProcess_GenSmoothNormals | aiPostProcessSteps::aiProcess_GenUVCoords | aiPostProcessSteps::aiProcess_Triangulate;

Model::Model(const std::string &path) {
  Assimp::Importer importer;
  auto             scene = importer.ReadFile(path.c_str(), modelProcessing);
  loadMesh(scene->mMeshes[0]);
}

Model::Model(const Model &copy) {
  m_vertices  = copy.getVertices();
  m_normals   = copy.getNormals();
  m_texcoords = copy.getTexcoords();
}

auto Model::getVertices() const -> const Vertices & {
  return m_vertices;
}

auto Model::getNormals() const -> const Normals & {
  return m_normals;
}

auto Model::getTexcoords() const -> const Texcoords & {
  return m_texcoords;
}

void Model::loadMesh(const aiMesh *mesh) {
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    m_vertices.push_back(FromAssimp::vec3<float>(mesh->mVertices[i]));
    m_normals.push_back(FromAssimp::vec3<float>(mesh->mNormals[i]));
    m_texcoords.push_back(FromAssimp::vec2<float>(mesh->mNormals[i]));
  }
}