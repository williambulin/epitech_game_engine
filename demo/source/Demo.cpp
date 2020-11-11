#include "Game.hpp"
#include "Components/Model.hpp"
#include "Components/Movement.hpp"
#include "Components/Physics.hpp"
#include "Components/Light.hpp"
#include "Systems/Physics.hpp"

#include <iostream>
#include <chrono>
#include <thread>

class Demo final : public Game {
public:
  bool                    m_holding{false};
  ECS::Admin::EntityIndex discoLight{0};
  ECS::Admin::EntityIndex discoLight2{0};

  explicit Demo(int argc = 0, char **argv = nullptr, char **env = nullptr) : Game{argc, argv, env} {
    std::cout << "Created Demo" << '\n';

    m_admin->createSystem<Systems::Physics>();

    auto audio{m_audioManager->createAudioSource("../resources/some.wav")};
    audio->setVolume(100);
    audio->play();

    using (auto entity{m_admin->createEntity()}) {
      // Physics
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
      (void)physics;
      // physics.applyLinearImpulse(ml::vec3{10.0f, 0.0f, 0.0f});
      physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, 4.0f});

      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      (void)transform;
      // transform.matrix.setTranslation(ml::vec3{-3.0f, -3.0f, -3.0f});

      // Model
      auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/viking_room.obj", "../resources/viking_room.png")};
      // auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/model-no-normals.obj", "../resources/model-texture.png")};
      (void)model;

      m_window->onMouseButton = [&](auto button, auto state) {
        if (button == Input::MouseButton::Left && state == Input::State::Pressed) {
          m_holding = true;
          std::cout << "Pressed" << '\n';
        } else if (button == Input::MouseButton::Left && state == Input::State::Released) {
          m_holding = false;
          std::cout << "Released" << '\n';
        }
      };

      m_window->onMouseMove = [&](float x, float y) {
        // std::cout << x << '\t' << y << '\n';
        // mouseX += x;
        // mouseY += y;
        // auto &physics{m_admin->getComponent<Components::Physics>(test)};
        if (m_holding)
          physics.applyAngularImpulse(ml::vec3{y / 20.0f, 0.0f, x / 20.0f});
      };
    }

    // using (auto entity{m_admin->createEntity()}) {
    //   // Transform
    //   auto &transform{m_admin->createComponent<Components::Transform>(entity)};
    //   transform.matrix.setTranslation(ml::vec3{0.0f, 1.0f, 0.0f});

    //   // Light
    //   auto &light{m_admin->createComponent<Components::Light>(entity)};
    //   light.type      = Components::Light::Type::Directional;
    //   light.color     = ml::vec3(1.0f, 1.0f, 1.0f);
    //   light.direction = ml::vec3(0.0f, 1.0f, 0.0f);
    //   (void)light;
    // }

    using (auto entity{m_admin->createEntity()}) {
      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      transform.matrix.setTranslation(ml::vec3{0.0f, 1.0f, 0.0f});

      // Light
      auto &light{m_admin->createComponent<Components::Light>(entity)};
      light.type      = Components::Light::Type::Directional;
      light.color     = ml::vec3(0.0f, 0.0f, 1.0f);
      light.direction = ml::vec3(-1.0f, 0.0f, 0.0f);
      (void)light;

      // Physics
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
      (void)physics;
      // physics.applyLinearImpulse(ml::vec3{10.0f, 0.0f, 0.0f});
      // physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, 1000.0f});
      discoLight = entity;
    }

    using (auto entity{m_admin->createEntity()}) {
      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      transform.matrix.setTranslation(ml::vec3{0.0f, 1.0f, 0.0f});

      // Light
      auto &light{m_admin->createComponent<Components::Light>(entity)};
      light.type      = Components::Light::Type::Directional;
      light.color     = ml::vec3(1.0f, 0.0f, 0.0f);
      light.direction = ml::vec3(1.0f, 0.0f, 0.0f);
      (void)light;

      // Physics
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
      (void)physics;
      // physics.applyLinearImpulse(ml::vec3{10.0f, 0.0f, 0.0f});
      // physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, 1000.0f});
      discoLight2 = entity;
    }

    // using (auto entity{m_admin.createEntity()}) {
    //   // Transform
    //   auto &transform{m_admin.createComponent<Components::Transform>(entity)};
    //   // transform.scale = glm::vec3(0.1f);
    //   transform.matrix.setTranslation(ml::vec3{-3.0f, -3.0f, -3.0f});

    //   // Model
    //   auto &model{m_admin.createComponent<Components::Model>(entity, renderer, "../resources/rock.obj", "../resources/rock.jpg")};
    //   (void)model;
    // }

    // using (auto entity{m_admin.createEntity()}) {
    //   // Transform
    //   auto &transform{m_admin.createComponent<Components::Transform>(entity)};
    //   // transform.scale = glm::vec3(0.1f);
    //   transform.matrix.setTranslation(ml::vec3{-3.0f, -3.0f, -3.0f});

    //   // Model
    //   auto &model{m_admin.createComponent<Components::Model>(entity, renderer, "../resources/disk.obj", "../resources/disk.jpg")};
    //   (void)model;
    // }
  }

  [[nodiscard]] bool update(float dt, std::uint64_t) final {
    // std::cout << mouseX << '\t' << mouseY << '\n';
    {
      auto &physics{m_admin->getComponent<Components::Physics>(discoLight)};
      physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, 10.0f * dt});
    }
    {
      auto &physics{m_admin->getComponent<Components::Physics>(discoLight2)};
      physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, 10.0f * dt});
    }
    return true;
  }

  ~Demo() final {
    std::cout << "Destroyed Demo" << '\n';
  }
};

int main(int argc, char **argv, char **env) {
  Demo demo{argc, argv, env};
  return demo.run();
}
