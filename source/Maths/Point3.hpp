#pragma once

class Point3 final {
public:
  float x{0.0f};
  float y{0.0f};
  float z{0.0f};

public:
  explicit Point3(float x, float y, float z) noexcept
    : x{x},
      y{y},
      z{z}
  {
  }

  explicit Point3(const Point3 &second) noexcept
    : x{second.x},
      y{second.y},
      z{second.z}
  {
  }

  [[nodiscard]] bool operator==(const Point3 &second) const noexcept {
    return(
      x == second.x &&
      y == second.y &&
      z == second.z
    );
  }
};