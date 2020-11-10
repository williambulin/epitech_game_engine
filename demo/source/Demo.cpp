#include "Game.hpp"
#include "Components/Model.hpp"
#include "Components/Movement.hpp"

#include <iostream>
#include <chrono>
#include <thread>

class Demo final : public Game {
public:
  explicit Demo(Vulkan::Renderer &renderer, ECS::Admin &admin, AudioManager &audioManager) : Game{renderer, admin, audioManager} {
    std::cout << "Created Demo" << '\n';

    auto audio{audioManager.createAudioSource("../resources/some.wav")};
    audio->setVolume(100);
    audio->play();

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
