
#include "Maths/Vec3.hpp"
#include "Physics/ICollisionSystem.hpp"

class AABB final : public ICollisionSystem<AABB> {
public:
  Vec3 topRight;
  Vec3 bottomLeft;

public:
  explicit AABB(Vec3 topRight, Vec3 bottomLeft) noexcept
    : topRight(topRight),
      bottomLeft(bottomLeft)
  {
  }

  explicit AABB(const AABB &second) noexcept
  : topRight(second.topRight),
    bottomLeft(second.bottomLeft)
  {
  }

  [[nodiscard]] bool Overlap(const AABB &second) noexcept
  {
    return(topRight.x > second.bottomLeft.x &&
    bottomLeft.x < second.topRight.x &&
    topRight.y > second.bottomLeft.y &&
    bottomLeft.y < second.topRight.y &&
    topRight.z > second.bottomLeft.z &&
    bottomLeft.z < second.topRight.z);
  }

  [[nodiscard]] bool operator==(const AABB &second) noexcept
  {
    return (bottomLeft == second.bottomLeft && topRight == second.topRight);
  }
};