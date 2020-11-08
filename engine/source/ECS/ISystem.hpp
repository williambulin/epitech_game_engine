#pragma once

#include <cstdint>

namespace ECS {
  class Admin;
  class ISystem;
}

class ECS::ISystem {
public:
  virtual ~ISystem() = default;

  virtual void cache()                                          = 0;
  virtual void update(float deltatime, std::uint64_t tickCount) = 0;
};
