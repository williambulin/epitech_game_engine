#pragma once

#include <vector>

#include "../Maths/Vectors.hpp"

class IAnimatedModel {
public:
	using JointsId	   = std::vector<Vector<unsigned int, 4>>;
	using JointsWeight = std::vector<Vector<float, 4>>;

public:
  [[nodiscard]] virtual const JointsId &getJointsId() const			= 0;
  [[nodiscard]] virtual const JointsWeight &getJointsWeight() const = 0;
};