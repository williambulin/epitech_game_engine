#include "Game.hpp"
#include "Components/Model.hpp"
#include "Components/Movement.hpp"
#include "Components/Physics.hpp"
#include "Systems/Physics.hpp"

#include <iostream>
#include <chrono>
#include <thread>

class Demo final : public Game {
public:
  explicit Demo(Vulkan::Renderer &renderer, ECS::Admin &admin) : Game{renderer, admin} {
    std::cout << "Created Demo" << '\n';

    m_admin.createSystem<Systems::Physics>();

    // using (auto entity{m_admin.createEntity()}) {
    //   // Transform
    //   auto &transform{m_admin.createComponent<Components::Transform>(entity)};
    //   // transform.scale = glm::vec3(0.1f);

    //   // Model
    //   auto &model{m_admin.createComponent<Components::Model>(entity, renderer, "../resources/rock.obj", "../resources/rock.jpg")};
    //   (void)model;
    // }

    // glm::perspective(0, 0, 0, 0);

    using (auto entity{m_admin.createEntity()}) {
      // Transform
      auto &transform{m_admin.createComponent<Components::Transform>(entity)};
      (void)transform;

      // Model
      auto &model{m_admin.createComponent<Components::Model>(entity, renderer, "../resources/viking_room.obj", "../resources/viking_room.png")};
      (void)model;

      // Physics
      auto &physics{m_admin.createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
      (void)physics;
      physics.applyLinearImpulse(ml::vec3{10.0f, 0.0f, 0.0f});
    }

    // using (auto entity{m_admin.createEntity()}) {
    //   // Transform
    //   auto &transform{m_admin.createComponent<Components::Transform>(entity)};
    //   // transform.scale    = glm::vec3(0.05f);
    //   // transform.position = glm::vec3(5.0f);

    //   // Model
    //   auto &model{m_admin.createComponent<Components::Model>(entity, renderer, "../resources/disk.obj", "../resources/disk.png")};
    //   (void)model;
    // }
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
