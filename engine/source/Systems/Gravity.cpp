#include "Gravity.hpp"

void Systems::Gravity::update(float dt, std::uint64_t) {
  auto &entities{getItems()};
  for (auto &&[entity, gravity, physics] : entities)
    if (!physics.getIsRigid() && gravity.hasGravity)
      physics.applyLinearImpulse(ml::vec3{0.0f, -9.81f * dt, 0.0f});
}