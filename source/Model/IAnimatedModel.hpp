#pragma once

#include <vector>

class IAnimatedModel {
public:
	using JointsId	   = std::vector<uvec4>;
	using JointsWeight = std::vector<vec4>;

public:
  [[nodiscard]] virtual const JointsId &getJointsId() const			= 0;
  [[nodiscard]] virtual const JointsWeight &getJointsWeight() const = 0;
};