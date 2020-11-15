#include "Gravity.hpp"

void Systems::Gravity::update(float dt, std::uint64_t) {
  auto &entities{getItems()};
  for (auto &&[entity, gravity, physics] : entities)
    if (!physics.getIsRigid())
      physics.applyLinearImpulse(ml::vec3{0.0f, -9.81f * dt * 5.0f, 0.0f});
}
