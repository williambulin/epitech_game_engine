#include "Game.hpp"
#include "Components/Model.hpp"
#include "Components/Movement.hpp"

#include <iostream>
#include <chrono>
#include <thread>

class MovementSystem final : public ECS::System<Components::Transform, Components::Movement> {
public:
  explicit MovementSystem(ECS::Admin &admin) : ECS::System<Components::Transform, Components::Movement>{admin} {}

  void update(float, std::uint64_t) final {
    auto &entities{getItems()};
    for (auto &&[ent, transform, movement] : entities)
      transform.position += movement.velocity;
  }
};

class Demo final : public Game {
public:
  explicit Demo(Vulkan::Renderer &renderer, ECS::Admin &admin) : Game{renderer, admin} {
    std::cout << "Created Demo" << '\n';

    m_admin.createSystem<MovementSystem>();

    using (auto entity{m_admin.createEntity()}) {
      // Transform
      auto &transform{m_admin.createComponent<Components::Transform>(entity)};
      transform.scale = glm::vec3(0.1f);

      // Model
      auto &model{m_admin.createComponent<Components::Model>(entity, renderer, "../resources/rock.obj", "../resources/rock.jpg")};
      (void)model;
    }

    using (auto entity{m_admin.createEntity()}) {
      // Transform
      auto &transform{m_admin.createComponent<Components::Transform>(entity)};
      (void)transform;

      // Model
      auto &model{m_admin.createComponent<Components::Model>(entity, renderer, "../resources/viking_room.obj", "../resources/viking_room.png")};
      (void)model;
    }

    using (auto entity{m_admin.createEntity()}) {
      // Transform
      auto &transform{m_admin.createComponent<Components::Transform>(entity)};
      transform.scale    = glm::vec3(0.05f);
      transform.position = glm::vec3(5.0f);

      // Model
      auto &model{m_admin.createComponent<Components::Model>(entity, renderer, "../resources/disk.obj", "../resources/disk.png")};
      (void)model;
    }
  }

  [[nodiscard]] bool update(float, std::uint64_t) final {
    return true;
  }

  ~Demo() final {
    std::cout << "Destroyed Demo" << '\n';
  }
};

#define HOST_GAME Demo
#include "GameHost.hpp"
