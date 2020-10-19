#include "AnimatedModel.hpp"

#include <algorithm>
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
  this->m_jointsId     = copy.getJointsId();
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

// Bone ---------------------------------------------------------------------------------------------------------------

AnimatedModel::Bone::Bone(const Id &id, const aiBone &bone) : m_id{id} {
  // todo : store transposed offsetmatrix with our own matrix type
  for (auto i = 0U; i < bone.mNumWeights; i++)
    m_vertexWeight.push_back(VertexWeight(bone.mWeights[i]));
}

auto AnimatedModel::Bone::getId() const -> const Id & {
  return m_id;
}

auto AnimatedModel::Bone::getOffsetMatrix() const -> const OffsetMatrix & {
  return m_offsetMatrix;
}

auto AnimatedModel::Bone::getVertexWeight() const -> const VertexWeightList & {
  return m_vertexWeight;
}

// Bone::VertexWeight -------------------------------------------------------------------------------------------------

AnimatedModel::Bone::VertexWeight::VertexWeight(const aiVertexWeight &vertexWeight) : m_id{vertexWeight.mVertexId}, m_weight{vertexWeight.mWeight} {}

auto AnimatedModel::Bone::VertexWeight::getId() const -> const Id & {
  return m_id;
}

auto AnimatedModel::Bone::VertexWeight::getWeight() const -> const Weight & {
  return m_weight;
}

// Node ---------------------------------------------------------------------------------------------------------------

AnimatedModel::Node::Node(const aiNode &node, const Parent &parent) : m_name{FromAssimp::String(node.mName)}, m_parent{parent} {
  // todo : store transposed transformmatrix with our own matrix type
  for (auto i = 0U; i < node.mNumChildren; i++)
    m_children.push_back(Node(*node.mChildren[i], this));
}

auto AnimatedModel::Node::getName() const -> const Name & {
  return m_name;
}

auto AnimatedModel::Node::getTransformMatrix() const -> const TransformMatrix & {
  return m_transformMatrix;
}

auto AnimatedModel::Node::getParent() const -> const Parent & {
  return m_parent;
}

auto AnimatedModel::Node::getChildren() const -> const Children & {
  return m_children;
}

// Animation ----------------------------------------------------------------------------------------------------------

AnimatedModel::Animation::Animation(const aiAnimation &animation) {
  float ticksPerSecond = animation.mTicksPerSecond > 0.0f ? animation.mTicksPerSecond : 1.0f;
  m_duration           = animation.mDuration / ticksPerSecond;
  for (auto i = 0U; i < animation.mNumChannels; i++) {
    auto channel       = animation.mChannels[i];
    auto nodeAnimation = AnimatedModel::Animation::Node(*channel, ticksPerSecond);
    m_nodeAnimation.insert(std::pair<std::string, AnimatedModel::Animation::Node>(FromAssimp::String(channel->mNodeName), nodeAnimation));
  }
}

auto AnimatedModel::Animation::getDuration() const -> const Duration & {
  return m_duration;
}

auto AnimatedModel::Animation::getLoop() const -> const Loop & {
  return m_loop;
}

auto AnimatedModel::Animation::getNodeAnimation() const -> const NodeAnimationMap & {
  return m_nodeAnimation;
}

// Animation::Node ----------------------------------------------------------------------------------------------------

AnimatedModel::Animation::Node::Node(const aiNodeAnim &channel, const float &ticksPerSecond) {}

auto AnimatedModel::Animation::Node::getPositionKey() const -> const VectorKeyMap & {
  return m_positionKey;
}

auto AnimatedModel::Animation::Node::getRotationKey() const -> const QuatKeyMap & {
  return m_rotationKey;
}

auto AnimatedModel::Animation::Node::getScalingKey() const -> const VectorKeyMap & {
  return m_scalingKey;
}

auto AnimatedModel::Animation::Node::getTransformMatrix(const float &timestamp) const -> Matrix<float, 4U, 4U> {
  auto position = getTransformation<VectorKey>(m_positionKey, timestamp).getTransformation();
  auto rotation = getTransformation<QuatKey>(m_rotationKey, timestamp).getTransformation();
  auto scaling  = getTransformation<VectorKey>(m_scalingKey, timestamp).getTransformation();
  // todo : return the matrix generated using these transformations
}

template <class T>
auto AnimatedModel::Animation::Node::getTransformation(const KeyMap<T> &keyMap, const float &timestamp) const -> T {
  std::vector<float> keyframeTimestamps;
  for (auto it = keyMap.begin(); it != keyMap.end(); it++)
    keyframeTimestamps.push_back(it->first);
  std::sort(keyframeTimestamps.begin(), keyframeTimestamps.end());
  for (auto i = 0U; i < keyframeTimestamps.size(); i++) {
    auto keyframeTimestamp = keyframeTimestamps[i];
    if (keyframeTimestamp > timestamp) {
      auto prevKeyframeTimestamp = keyframeTimestamps[(i == 0 ? keyframeTimestamps.size() : i) - 1];
      return keyMap[prevKeyframeTimestamp].interpolate(keyMap[keyframeTimestamp], timestamp);
    }
  }
  return keyMap[keyframeTimestamps[0]];
}

// Animation::Node::TransformationKey ---------------------------------------------------------------------------------

template <class T>
AnimatedModel::Animation::Node::TransformationKey<T>::TransformationKey(const Timestamp &timestamp, const Transformation &transformation) : m_timestamp{timestamp},
                                                                                                                                            m_transformation{transformation} {}

template <class T>
auto AnimatedModel::Animation::Node::TransformationKey<T>::getTimestamp() const -> const Timestamp & {
  return m_timestamp;
}

template <class T>
auto AnimatedModel::Animation::Node::TransformationKey<T>::getTransformation() const -> const Transformation & {
  return m_transformation;
}

template <class T>
auto AnimatedModel::Animation::Node::TransformationKey<T>::getProgress(const TransformationKey &other) const -> float {
  return std::max<float>(0.0, std::min<float>(1.0, (timestamp - m_timestamp) / (other.getTimestamp() - m_timestamp)));
}

// Animation::Node::VectorKey -----------------------------------------------------------------------------------------

AnimatedModel::Animation::Node::VectorKey::VectorKey(const aiVectorKey &vectorKey, const float &ticksPerSecond) {
  TransformationKey::TransformationKey(vectorKey.mTime / ticksPerSecond, FromAssimp::Vector3(vectorKey.mValue));
}

auto AnimatedModel::Animation::Node::VectorKey::interpolate(const VectorKey &other, const float &timestamp) -> Vector<float, 3U> {
  return m_transformation.lerp(other.getTransformation(), getProgress(other));
}

// Animation::Node::QuatKey -------------------------------------------------------------------------------------------

AnimatedModel::Animation::Node::QuatKey::QuatKey(const aiQuatKey &quatKey, const float &ticksPerSecond) {
  TransformationKey::TransformationKey(quatKey.mTime / ticksPerSecond, FromAssimp::Quaternion(quatKey.mValue));
}

auto AnimatedModel::Animation::Node::QuatKey::interpolate(const QuatKey &other, const float &timestamp) const -> Quaternion {
  return Quaternion::slerp(m_transformation, other.getTransformation(), getProgress(other));
}