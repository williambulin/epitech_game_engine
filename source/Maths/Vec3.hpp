class Vec3 {
public:

  Vec3(float x, float y, float z) noexcept
      : x(x),
        y(y),
        z(z)
  {
  }

  Vec3(const Vec3 &second) noexcept
    : x(second.x),
      y(second.y),
      z(second.z)
  {
  }

  bool operator==(const Vec3 &second) {
    return(
      x == second.x &&
      y == second.y &&
      z == second.z
    );
  }

  float x;
  float y;
  float z;
};