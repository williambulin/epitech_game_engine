#include "AnimatedModel.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdexcept>
#include <map>
#include <string>

static const unsigned int modelProcessing = aiPostProcessSteps::aiProcess_CalcTangentSpace | aiPostProcessSteps::aiProcess_GenSmoothNormals | aiPostProcessSteps::aiProcess_GenUVCoords | aiPostProcessSteps::aiProcess_LimitBoneWeights | aiPostProcessSteps::aiProcess_Triangulate;

AnimatedModel::AnimatedModel(const std::string &path) {
  Assimp::Importer importer;
  auto             scene = importer.ReadFile(path.c_str(), modelProcessing);
  if (!loadAnimation(scene->mMeshes[0], scene))
    throw std::runtime_error(path + ": Model mesh does not support animation.");
  loadMesh(scene->mMeshes[0]);
}

AnimatedModel::AnimatedModel(const AnimatedModel &copy) {
  Model::Model(copy);
  this->m_jointsId = copy.getJointsId();
  this->m_jointsWeight = copy.getJointsWeight();
}

auto AnimatedModel::getJointsId() const -> const JointsId & {
  return m_jointsId;
}

auto AnimatedModel::getJointsWeight() const -> const JointsWeight & {
  return m_jointsWeight;
}

bool AnimatedModel::loadAnimation(const aiMesh *mesh, const aiScene *scene) {
  if (!mesh->HasBones())
    return false;
  for (unsigned int i = 0; i < mesh->mNumBones; i++) {
    
  }
}