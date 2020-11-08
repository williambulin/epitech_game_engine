#pragma once

#include <memory>

#include "Global/ApplicationBase.hpp"
#include "Extension/Libraries/Vulkan.hpp"

class Core final : public ApplicationBase {
public:
  explicit Core(int argc = 0, char **argv = nullptr, char **env = nullptr) : ApplicationBase{argc, argv, env} {}

  [[nodiscard]] bool start() final;
};
