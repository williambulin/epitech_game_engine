#pragma once

#include <assimp\scene.h>
#include <map>
#include <string>
#include <vector>

#include "../Maths/Matrix.hpp"
#include "../Maths/Quaternion.hpp"
#include "../Maths/Vectors.hpp"
#include "Converter/Assimp.hpp"
#include "IAnimatedModel.hpp"
#include "Model.hpp"

class AnimatedModel : public IAnimatedModel, public Model {
private:
  class Bone final {
  private:
    class VertexWeight final {
    public:
      // AnimatedModel::Bone::VertexWeight ----------------------------------------------------------------------------
      using Id     = unsigned int;
      using Weight = float;

    private:
      Id     m_id{};
      Weight m_weight{};

    public:
      explicit VertexWeight(const aiVertexWeight &);
      ~VertexWeight() = default;

      [[nodiscard]] auto getId() const -> const Id &;
      [[nodiscard]] auto getWeight() const -> const Weight &;
    };

  public:
    // AnimatedModel::Bone --------------------------------------------------------------------------------------------
    using Id               = unsigned int;
    using OffsetMatrix     = Matrix<float, 4U, 4U>;
    using VertexWeightList = std::vector<VertexWeight>;

  private:
    Id               m_id{};
    OffsetMatrix     m_offsetMatrix{};
    VertexWeightList m_vertexWeight{};

  public:
    explicit Bone(const Id &, const aiBone &);
    ~Bone() = default;

    [[nodiscard]] auto getId() const -> const Id &;
    [[nodiscard]] auto getOffsetMatrix() const -> const OffsetMatrix &;
    [[nodiscard]] auto getVertexWeight() const -> const VertexWeightList &;
  };

  class Node final {
  public:
    // AnimatedModel::Node --------------------------------------------------------------------------------------------
    using Name            = std::string;
    using TransformMatrix = Matrix<float, 4U, 4U>;
    using Parent          = Node *;
    using Children        = std::vector<Node>;

  private:
    Name            m_name{};
    TransformMatrix m_transformMatrix{};
    Parent          m_parent{};
    Children        m_children{};

  public:
    explicit Node(const aiNode &, const Parent &);
    ~Node() = default;

    [[nodiscard]] auto getName() const -> const Name &;
    [[nodiscard]] auto getTransformMatrix() const -> const TransformMatrix &;
    [[nodiscard]] auto getParent() const -> const Parent &;
    [[nodiscard]] auto getChildren() const -> const Children &;
  };

  class Animation final {
  private:
    class Node final {
    private:
      template <class T>
      class TransformationKey {
      public:
        // AnimatedModel::Animation::Node::TransformationKey ----------------------------------------------------------
        using Timestamp      = float;
        using Transformation = T;

      protected:
        Timestamp      m_timestamp;
        Transformation m_transformation;

      public:
        explicit TransformationKey(const Timestamp &, const Transformation &);
        ~TransformationKey() = default;

        [[nodiscard]] auto getTimestamp() const -> const Timestamp &;
        [[nodiscard]] auto getTransformation() const -> const Transformation &;

      protected:
        [[nodiscard]] auto getProgress(const TransformationKey &) const -> Timestamp;
      };

      class VectorKey final : public TransformationKey<Vector<float, 3U>> {
      public:
        // AnimatedModel::Animation::Node::VectorKey ------------------------------------------------------------------
        explicit VectorKey(const aiVectorKey &, const float &);
        ~VectorKey() = default;

        [[nodiscard]] auto interpolate(const VectorKey &, const float &) -> Vector<float, 3U>;
      };

      class QuatKey final : public TransformationKey<Quaternion> {
      public:
        // AnimatedModel::Animation::Node::QuatKey --------------------------------------------------------------------
        explicit QuatKey(const aiQuatKey &, const float &);
        ~QuatKey() = default;

        [[nodiscard]] auto interpolate(const QuatKey &, const float &) const -> Quaternion;
      };

    public:
      // AnimatedModel::Animation::Node -------------------------------------------------------------------------------
      template <class T>
      using KeyMap       = std::map<float, T>;
      using VectorKeyMap = KeyMap<VectorKey>;
      using QuatKeyMap   = KeyMap<QuatKey>;

    private:
      VectorKeyMap m_positionKey{};
      QuatKeyMap   m_rotationKey{};
      VectorKeyMap m_scalingKey{};

    public:
      explicit Node(const aiNodeAnim &, const float &);
      ~Node() = default;

      [[nodiscard]] auto getPositionKey() const -> const VectorKeyMap &;
      [[nodiscard]] auto getRotationKey() const -> const QuatKeyMap &;
      [[nodiscard]] auto getScalingKey() const -> const VectorKeyMap &;
      [[nodiscard]] auto getTransformMatrix(const float &) const -> Matrix<float, 4U, 4U>;

    private:
      template <class T>
      [[nodiscard]] auto getTransformation(const KeyMap<T> &, const float &) const -> T;
    };

  public:
    // AnimatedModel::Animation ---------------------------------------------------------------------------------------
    using Duration         = float;
    using Loop             = bool;
    using NodeAnimationMap = std::map<std::string, Node>;

  private:
    Duration         m_duration{};
    Loop             m_loop{};
    NodeAnimationMap m_nodeAnimation{};

  public:
    explicit Animation(const aiAnimation &);
    ~Animation() = default;

    [[nodiscard]] auto getDuration() const -> const Duration &;
    [[nodiscard]] auto getLoop() const -> const Loop &;
    [[nodiscard]] auto getNodeAnimation() const -> const NodeAnimationMap &;
  };

public:
  // AnimatedModel ----------------------------------------------------------------------------------------------------
  using BoneMap                = std::map<std::string, Bone>;
  using InverseTransformMatrix = Matrix<float, 4U, 4U>;
  using Node                   = Node;
  using AnimationMap           = std::map<std::string, Animation>;

private:
  BoneMap                m_boneMap{};
  InverseTransformMatrix m_inverseTransform{};
  Node                   m_nodeData{};
  AnimationMap           m_animationMap{};

protected:
  JointsId     m_jointsId{};
  JointsWeight m_jointsWeight{};

public:
  explicit AnimatedModel(const std::string &);
  AnimatedModel(const AnimatedModel &);
  ~AnimatedModel = default;

  [[nodiscard]] auto getJointsId() const -> const JointsId &;
  [[nodiscard]] auto getJointsWeight() const -> const JointsWeight &;

protected:
  bool loadAnimation(const aiMesh *, const aiScene *);
};