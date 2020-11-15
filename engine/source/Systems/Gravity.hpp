#pragma once

#include "ECS/System.hpp"
#include "Components/Gravity.hpp"
#include "Components/Physics.hpp"

namespace Systems {
  class Gravity;
}

class Systems::Gravity final : public ECS::System<Components::Gravity, Components::Physics> {
public:
    DLLATTRIB explicit Gravity(ECS::Admin &admin) : ECS::System<Components::Gravity, Components::Physics>{admin} {}
    DLLATTRIB void update(float dt, std::uint64_t) final;
};