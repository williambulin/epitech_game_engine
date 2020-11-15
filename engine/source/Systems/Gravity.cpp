#include "Gravity.hpp"

void Systems::Gravity::update(float dt, std::uint64_t) {
  auto &entities{getItems()};
  for (auto &&[entity, gravity, physics] : entities)
    if (!physics.getIsRigid() && gravity.hasGravity) {
      auto tmp = physics.getLinearVelocity();

      if (tmp.y > 0) {
        tmp.y += -9.81f;
        tmp.y *= .75f;
      } else if (tmp.y < 0) {
        tmp.y += -9.81f;
        tmp.y *= 1.5f;
      } else
        tmp.y += -9.81f;
    }
}
