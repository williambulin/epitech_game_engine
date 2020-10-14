#pragma once

#include <assimp\scene.h>
#include <map>
#include <pair>
#include <string>
#include <vector>

#include "../Maths/Vectors.hpp"
#include "Converter/Assimp.hpp"
#include "IAnimatedModel.hpp"
#include "Model.hpp"

class AnimatedModel : public IAnimatedModel, public Model {
private:
  struct Bone final {
  private:
    struct VertexWeight final {
    public:
      using Id     = unsigned int;
      using Weight = float;

    private:
      Id     m_id{};
      Weight m_weight{};

    public:
      explicit VertexWeight(const aiVertexWeight &vertexWeight) : m_id{vertexWeight.mVertexId}, m_weight{vertexWeight.mWeight} {}

      ~VertexWeight() = default;

      [[nodiscard]] auto getId() const -> const Id & {
        return m_id;
      }

      [[nodiscard]] auto getWeight() const -> const Weight & {
        return m_weight;
      }
    };

  public:
    using Id               = unsigned int;
    using OffsetMatrix     = int;  // todo : use our own matrix type
    using VertexWeightList = std::vector<VertexWeight>;

  private:
    Id               m_id{};
    OffsetMatrix     m_offsetMatrix{};
    VertexWeightList m_vertexWeight{};

  public:
    explicit Bone(const Id &id, const aiBone &bone) : m_id{id} {
      // todo : store transposed offsetmatrix with our own matrix type
      for (auto i = 0U; i < bone.mNumWeights; i++)
        m_vertexWeight.push_back(VertexWeight(bone.mWeights[i]));
    }

    ~Bone() = default;

    [[nodiscard]] auto getId() const -> const Id & {
      return m_id;
    }
    [[nodiscard]] auto getOffsetMatrix() const -> const OffsetMatrix & {
      return m_offsetMatrix;
    }

    [[nodiscard]] auto getVertexWeight() const -> const VertexWeightList & {
      return m_vertexWeight;
    }
  };

  struct Node final {
  public:
    using Name            = std::string;
    using TransformMatrix = int;  // todo : use our own matrix type
    using Parent          = Node *;
    using Children        = std::vector<Node>;

  private:
    Name            m_name{};
    TransformMatrix m_transformMatrix{};
    Parent          m_parent{};
    Children        m_children{};

  public:
    explicit Node(const aiNode &node, const Parent &parent) : m_name{node.mName.C_Str()}, m_parent{parent} {
      // todo : store transposed transformmatrix with our own matrix type
      for (auto i = 0U; i < node.mNumChildren; i++)
        m_children.push_back(Node(*node.mChildren[i], this));
    }

    ~Node() = default;

    [[nodiscard]] auto getName() const -> const Name & {
      return m_name;
    }

    [[nodiscard]] auto getTransformMatrix() const -> const TransformMatrix & {
      return m_transformMatrix;
    }

    [[nodiscard]] auto getParent() const -> const Parent & {
      return m_parent;
    }

    [[nodiscard]] auto getChildren() const -> const Children & {
      return m_children;
    }
  };

  struct Animation final {
  private:
    struct Node final {
    private:
      template <class T>
      struct TransformationKey {
      public:
        using Timestamp      = float;
        using Transformation = T;

      private:
        Timestamp      m_timestamp;
        Transformation m_transformation;

      public:
        explicit TransformationKey(const Timestamp &timestamp, const Transformation &transformation) : m_timestamp{timestamp}, m_transformation{transformation} {}

        ~TransformationKey() = default;

        [[nodiscard]] auto getTimestamp() const -> const Timestamp & {
          return m_timestamp;
        }

        [[nodiscard]] auto getTransformation() const -> const Transformation & {
          return m_transformation;
        }
      };

      struct VectorKey final : public TransformationKey<Vector<float, 3U>> {
      public:
        explicit VectorKey(const aiVectorKey &vectorKey, const float &ticksPerSecond) {
          TransformationKey::TransformationKey(vectorKey.mTime / ticksPerSecond, FromAssimp::Vector3(vectorKey.mValue));
        }

        ~VectorKey() = default;

        [[nodiscard]] auto interpolate(const Vector<float, 3U> &other, const float &timestamp) const -> Vector<float, 3U> {
          // todo interpolate vectors
        }
      };

      /*
      struct QuatKey final : public TransformationKey<Quaternion<float>> {
      public:
        explicit QuatKey::QuatKey(const aiQuatKey &quatKey, const float &ticksPerSecond) {
          TransformationKey::TransformationKey(quatKey.mTime / ticksPerSecond, FromAssimp::Quaternion(quatKey.mValue));
        }

        QuatKey::~QuatKey() = default;

        [[nodiscard]] auto interpolate(const Quaternion<float> &other, const float &timestamp) const -> Quaternion<float> {
          //todo interpolate quaternions
        }
      };
      */

    public:
      using VectorKeyMap = std::map<float, VectorKey>;
      // using QuatKeyMap = std::map<float, QuatKey>;

    private:
      VectorKeyMap m_positionKey{};
      // QuatKeyMap   m_rotationKey{};
      VectorKeyMap m_scalingKey{};

    public:
      explicit Node(const aiAnimation &channel, const float &ticksPerSecond) {}

      ~Node() = default;

      [[nodiscard]] auto getPositionKey() const -> const VectorKeyMap & {
        return m_positionKey;
      }

      /*
      [[nodiscard]] auto getRotationKey() const -> const QuatKeyMap & {
        return m_rotationKey;
      }
      */

      [[nodiscard]] auto getScalingKey() const -> const VectorKeyMap & {
        return m_scalingKey;
      }
    };

  public:
    using Duration         = float;
    using NodeAnimationMap = std::map<std::string, Node>;

  private:
    Duration         m_duration{};
    NodeAnimationMap m_nodeAnimation{};

  public:
    explicit Animation(const aiAnimation &animation) {
      float ticksPerSecond = animation.mTicksPerSecond > 0.0f ? animation.mTicksPerSecond : 1.0f;
      m_duration           = animation.mDuration / ticksPerSecond;
      for (auto i = 0U; i < animation.mNumChannels; i++) {
        auto channel = animation.mChannels[i];
        // auto nodeAnimation = AnimatedModel::Animation::Node(channel, ticksPerSecond);
        // m_nodeAnimation.insert(std::pair<std::string, AnimatedModel::Animation::Node>(channel->mNodeName, nodeAnimation));
      }
    }

    ~Animation() = default;
  };

  using BoneMap                = std::map<std::string, Bone>;
  using InverseTransformMatrix = int;  // todo : use our own matrix type
  using Node                   = Node;
  using AnimationMap           = std::map<std::string, Animation>;

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