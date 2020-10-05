
#include <Maths/Vec3.hpp>
#include <Physics/ICollisionSystem.hpp>

class AABB : IColisionSystem<AABB> {
public:
  bool Overlap(const AABB &second) noexcept
  {
    return(top_right.x > second.bottom_left.x &&
    bottom_left.x < second.top_right.x &&
    top_right.y > second.bottom_left.y &&
    bottom_left.y < second.top_right.y &&
    top_right.z > second.bottom_left.z &&
    bottom_left.z < second.top_right.z);
  }

  bool operator==(const AABB &second)
  {
    return (bottom_left == second.bottom_left && top_right == second.top_right);
  }

  Vec3 top_right;
  Vec3 bottom_left;

};