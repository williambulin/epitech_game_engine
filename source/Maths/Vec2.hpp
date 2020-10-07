#pragma once

#include <vector>

class Vec2 final {
public:
  float x{0.0f};
  float y{0.0f};

public:
  explicit Vec2(float x, float y) noexcept : x{x}, y{y} {}

  explicit Vec2(std::vector<float> &data) : x{data[0]}, y{data[1]} {}

  explicit Vec2(const Vec2 &second) noexcept : x{second.x}, y{second.y} {}

  [[nodiscard]] bool operator==(const Vec2 &second) const noexcept {
    return (x == second.x && y == second.y);
  }
};