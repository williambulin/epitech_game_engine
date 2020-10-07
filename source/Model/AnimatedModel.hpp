#pragma once

#include <vector>

#include "IAnimatedModel.hpp"
#include "Model.hpp"

class AnimatedModel : public IAnimatedModel, public Model {
protected:
  JointsId	   m_jointsId{};
  JointsWeight m_jointsWeight{};

public:
  [[nodiscard]] auto getJointsId() const -> const JointsId &;
  [[nodiscard]] auto getJointsWeight() const -> const JointsWeight &;
};