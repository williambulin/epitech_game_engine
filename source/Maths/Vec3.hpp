#pragma once

class Vec3 final {
public:
  float x{0.0f};
  float y{0.0f};
  float z{0.0f};

public:
  explicit Vec3(float x, float y, float z) noexcept
    : x{x},
      y{y},
      z{z}
  {
  }

  Vec3(const Vec3 &second) noexcept
    : x{second.x},
      y{second.y},
      z{second.z}
  {
  }

  [[nodiscard]] bool operator==(const Vec3 &second) const noexcept {
    return(
      x == second.x &&
      y == second.y &&
      z == second.z
    );
  }

  [[nodiscard]] Vec3 operator-(const Vec3 & rhs) const noexcept {
    return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
  }
  
  // DOT PRODUCT
  [[nodiscard]] float operator*(const Vec3 & rhs) const noexcept {
    return{ x * rhs.x + y * rhs.y + z * rhs.z };
  }

  // CROSS PRODUCT
  [[nodiscard]] Vec3 operator^(const Vec3 & rhs) const noexcept {
    return Vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
  }

  [[nodiscard]] Vec3 operator*(const float& rhs) const noexcept {
    return Vec3(x * rhs, y * rhs, z * rhs);
  }
};