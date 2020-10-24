#pragma once
#define MAX_JOINT_TRANSFORM 255
#define MAX_JOINT_WEIGHT    4

#include <string>
#include <vector>

#include "../Maths/Matrix.hpp"
#include "../Maths/Vectors.hpp"

class IAnimatedModel {
public:
  using JointsId        = std::vector<Vector<unsigned int, MAX_JOINT_TRANSFORM>>;
  using JointsTransform = std::array<Matrix<float, 4U, 4U>, MAX_JOINT_TRANSFORM>;
  using JointsWeight    = std::vector<Vector<float, MAX_JOINT_WEIGHT>>;

public:
  [[nodiscard]] virtual const JointsId &    getJointsId() const     = 0;
  [[nodiscard]] virtual JointsTransform     getJointsTransform()    = 0;
  [[nodiscard]] virtual const JointsWeight &getJointsWeight() const = 0;
};