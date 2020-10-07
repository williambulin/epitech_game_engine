#include "AnimatedModel.hpp"

auto AnimatedModel::getJointsId() const -> const JointsId & {
  return m_jointsId;
}

auto AnimatedModel::getJointsWeight() const -> const JointsWeight & {
  return m_jointsWeight;
}