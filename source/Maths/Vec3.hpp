class Vec3 final {
public:
  float x;
  float y;
  float z;  

public:
  explicit Vec3(float x, float y, float z) noexcept
      : x(x),
        y(y),
        z(z)
  {
  }

  explicit Vec3(const Vec3 &second) noexcept
    : x(second.x),
      y(second.y),
      z(second.z)
  {
  }

  [[nodiscard]] bool operator==(const Vec3 &second) noexcept {
    return(
      x == second.x &&
      y == second.y &&
      z == second.z
    );
  }
};