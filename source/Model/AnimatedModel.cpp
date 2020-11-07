#include "AnimatedModel.hpp"

#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <ctime>
#include <stdexcept>
#include <map>
#include <math.h>
#include <string>

#include "../Maths/Matrix.hpp"
#include "../Maths/Quaternion.hpp"
#include "../Maths/Vectors.hpp"
#include "Converter/Assimp.hpp"

static const unsigned int modelProcessing = aiPostProcessSteps::aiProcess_CalcTangentSpace | aiPostProcessSteps::aiProcess_GenSmoothNormals | aiPostProcessSteps::aiProcess_GenUVCoords | aiPostProcessSteps::aiProcess_LimitBoneWeights | aiPostProcessSteps::aiProcess_Triangulate;

AnimatedModel::AnimatedModel(const std::string &path) {
  Assimp::Importer importer;
  auto             data = importer.ReadFile(path.c_str(), modelProcessing);
  if (!loadSkeleton(data->mMeshes[0], data->mRootNode))
    throw std::runtime_error(path + ": Model mesh does not support animation.");
  loadMesh(data->mMeshes[0]);
  loadAnimation(data);
}

AnimatedModel::AnimatedModel(const AnimatedModel &copy) : Model{copy} {
  this->m_jointsId     = copy.getJointsId();
  this->m_jointsWeight = copy.getJointsWeight();
}

void AnimatedModel::animate(const std::string &animationName, const float &animationStrength, const bool &loopAnimation) {
  auto name = animationName.size() == 0 ? m_animationMap.begin()->first : animationName;
  if (m_animationMap.find(name) == m_animationMap.end())
    throw std::runtime_error(name + ": Animation does not exist for the specified model.");
  m_animationBuffer.push_back(std::pair<std::string, float>{name, animationStrength});
  auto animation = m_animationMap[name];
  animation.setLoop(loopAnimation);
  if (!animation.isStarted())
    animation.start();
}

auto AnimatedModel::getAnimationList() const -> std::vector<std::string> {
  std::vector<std::string> animationList;
  for (auto it = m_animationMap.begin(); it != m_animationMap.end(); it++)
    animationList.push_back(it->first);
  return animationList;
}

auto AnimatedModel::getJointsId() const -> const JointsId & {
  return m_jointsId;
}

auto AnimatedModel::getJointsTransform() -> JointsTransform {
  JointsTransform jointsTransform;
  for (auto i = 0U; i < MAX_JOINT_TRANSFORM; i++)
    jointsTransform[i] = Matrix4<float>{1};
  computeAnimation(jointsTransform, m_nodeData);
  m_animationBuffer.clear();
  return jointsTransform;
}

auto AnimatedModel::getJointsWeight() const -> const JointsWeight & {
  return m_jointsWeight;
}

void AnimatedModel::loadAnimation(const std::string &path) {
  Assimp::Importer importer;
  auto             scene = importer.ReadFile(path.c_str(), modelProcessing);
  loadAnimation(scene);
}

void AnimatedModel::computeAnimation(JointsTransform &jointsTransform, const Node &node, const Matrix<float, 4U, 4U> &parentTransform) const {
  // Compute node transformation
  Matrix<float, 4U, 4U> nodeTransform{node.getTransformMatrix()};
  for (auto &animationData : m_animationBuffer) {
    auto &animation     = const_cast<Animation &>(m_animationMap.at(animationData.first));
    auto  nodeAnimation = animation.getNodeAnimation().find(node.getName());
    if (nodeAnimation != animation.getNodeAnimation().end())
      nodeTransform = Matrix<float, 4U, 4U>::mix(nodeTransform, (*nodeAnimation).second.getTransformMatrix(animation.getTime()), animationData.second);
  }

  // Apply node transformation to the joint transformations
  auto globalTransform = parentTransform * nodeTransform;
  auto bone            = m_boneMap.find(node.getName());
  if (bone != m_boneMap.end())
    jointsTransform[(*bone).second.getId()] = m_inverseTransform * globalTransform * (*bone).second.getOffsetMatrix();

  // Compute it for the child nodes
  for (auto &child : node.getChildren())
    computeAnimation(jointsTransform, child, globalTransform);
}

void AnimatedModel::loadAnimation(const aiScene *data) {
  for (auto i = 0U; i < data->mNumAnimations; i++) {
    auto &animation = data->mAnimations[i];
    m_animationMap.emplace(std::make_pair(FromAssimp::str(animation->mName), Animation{*animation}));
  }
}

bool AnimatedModel::loadSkeleton(const aiMesh *mesh, const aiNode *rootNode) {
  if (!mesh->HasBones())
    return false;
  for (auto i = 0U; i < mesh->mNumBones; i++) {
    auto &bone = mesh->mBones[i];
    m_boneMap.emplace(std::make_pair(FromAssimp::str(bone->mName), Bone{i, *bone}));
  }
  m_inverseTransform = FromAssimp::mat4(rootNode->mTransformation);
  m_nodeData         = Node{*rootNode};
  return true;
}

// Bone ---------------------------------------------------------------------------------------------------------------

AnimatedModel::Bone::Bone(const Id &id, const aiBone &bone) : m_id{id} {
  m_offsetMatrix = FromAssimp::mat4(bone.mOffsetMatrix);
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

AnimatedModel::Node::Node(const aiNode &node, const Parent &parent) : m_name{FromAssimp::str(node.mName)}, m_parent{parent} {
  m_transformMatrix = FromAssimp::mat4(node.mTransformation);
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

AnimatedModel::Animation::Animation(const aiAnimation &animation) : m_clock{-1}, m_loop{false}, m_time{0.0f} {
  float ticksPerSecond = animation.mTicksPerSecond > 0.0f ? animation.mTicksPerSecond : 1.0f;
  m_duration           = animation.mDuration / ticksPerSecond;
  for (auto i = 0U; i < animation.mNumChannels; i++) {
    auto &channel = animation.mChannels[i];
    m_nodeAnimation.emplace(std::make_pair(FromAssimp::str(channel->mNodeName), Node{*channel, ticksPerSecond}));
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

auto AnimatedModel::Animation::getTime() -> const Time {
  if (m_clock == -1)
    return 0.0f;
  auto currentTime = std::clock();
  m_time += (currentTime - m_clock) / CLOCKS_PER_SEC;
  m_clock = currentTime;
  if (m_time > m_duration)
    m_time = m_loop ? std::fmod<float>(m_time, m_duration) : m_duration;
  return m_time;
}

auto AnimatedModel::Animation::isComplete() -> bool {
  return m_clock == -1 || m_loop ? false : getTime() >= m_duration;
}

auto AnimatedModel::Animation::isStarted() const -> bool {
  return m_clock != -1;
}

void AnimatedModel::Animation::reset() {
  m_time = 0.0f;
}

void AnimatedModel::Animation::setLoop(const Loop &loop) {
  m_loop = loop;
}

void AnimatedModel::Animation::start() {
  m_clock = std::clock();
}

void AnimatedModel::Animation::stop() {
  m_clock = -1;
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

auto AnimatedModel::Animation::Node::getTransformMatrix(const float &timestamp) const -> Matrix4<float> {
  auto position        = getInterpolatedFrame<VectorKey>(m_positionKey, timestamp).getTransformation();     // Compute the interpolated position frame
  auto transformMatrix = Matrix4<float>::translate(Vector3<float>{position[0], position[1], position[2]});  // Apply it to the tranform matrix
  auto rotation        = getInterpolatedFrame<QuatKey>(m_rotationKey, timestamp).getTransformation();       // And repeat for rotation and scaling
  transformMatrix *= rotation.toRotationMatrix();
  auto scaling = getInterpolatedFrame<VectorKey>(m_scalingKey, timestamp).getTransformation();
  return transformMatrix.scale(scaling[0], scaling[1], scaling[2]);
}

template <class T>
auto AnimatedModel::Animation::Node::getInterpolatedFrame(const KeyMap<T> &keyMap, const float &timestamp) const -> T {
  std::vector<float> keyframeTimestamps;
  for (auto it = keyMap.begin(); it != keyMap.end(); it++)
    keyframeTimestamps.push_back(it->first);                        // Gather the keyframes' timestamps
  std::sort(keyframeTimestamps.begin(), keyframeTimestamps.end());  // And sort them
  for (auto i = 0U; i < keyframeTimestamps.size(); i++) {
    auto &keyframeTimestamp = keyframeTimestamps[i];
    if (keyframeTimestamp > timestamp) {                                                               // Detect the frame just after our timestamp
      auto &prevKeyframeTimestamp = keyframeTimestamps[(i == 0 ? keyframeTimestamps.size() : i) - 1];  // Get the frame just before our timestamp
      return keyMap.at(prevKeyframeTimestamp).interpolate(keyMap.at(keyframeTimestamp), timestamp);    // And return the interpolation between the two
    }
  }
  return keyMap.at(keyframeTimestamps[0]);  // This line should never be reach but if it does we return the first frame
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
auto AnimatedModel::Animation::Node::TransformationKey<T>::getProgress(const TransformationKey &other, const float &timestamp) const -> float {
  return std::max<float>(0.0, std::min<float>(1.0, (timestamp - m_timestamp) / (other.getTimestamp() - m_timestamp)));
}

// Animation::Node::VectorKey -----------------------------------------------------------------------------------------

AnimatedModel::Animation::Node::VectorKey::VectorKey(const aiVectorKey &vectorKey, const float &ticksPerSecond) : TransformationKey(vectorKey.mTime / ticksPerSecond, FromAssimp::vec3(vectorKey.mValue)) {}

auto AnimatedModel::Animation::Node::VectorKey::interpolate(const VectorKey &other, const float &timestamp) const -> VectorKey {
  return VectorKey{timestamp, m_transformation.lerp(other.getTransformation(), getProgress(other, timestamp))};
}

// Animation::Node::QuatKey -------------------------------------------------------------------------------------------

AnimatedModel::Animation::Node::QuatKey::QuatKey(const aiQuatKey &quatKey, const float &ticksPerSecond) : TransformationKey(quatKey.mTime / ticksPerSecond, FromAssimp::quat(quatKey.mValue)) {}

auto AnimatedModel::Animation::Node::QuatKey::interpolate(const QuatKey &other, const float &timestamp) const -> QuatKey {
  return QuatKey{timestamp, Quaternion::slerp(m_transformation, other.getTransformation(), getProgress(other, timestamp))};
}