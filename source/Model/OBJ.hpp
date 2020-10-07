#pragma once

#include <string>

#include "Model.hpp"

class OBJ final : public Model {
public:
  explicit OBJ(const std::string &);
  OBJ(const OBJ &);
};