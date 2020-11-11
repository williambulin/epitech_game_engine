#pragma once

#include <cstdint>

namespace Input {
  enum class State : std::uint8_t;
}

enum class Input::State : std::uint8_t {
  Unknown,
  Pressed,
  Released,
  Repeating,
};
