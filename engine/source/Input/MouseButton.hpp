#pragma once

#include <cstdint>

namespace Input {
  enum class MouseButton : std::uint8_t;
}

enum class Input::MouseButton : std::uint8_t {
  Left,
  Middle,
  Right,
  Up,
  Down,
  LastMouseButton,
};
