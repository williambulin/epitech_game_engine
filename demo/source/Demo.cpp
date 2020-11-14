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
  ECS::Admin::EntityIndex test{0};
  ECS::Admin::EntityIndex discoLight{0};
  ECS::Admin::EntityIndex discoLight2{0};

  void onMouseButton(Input::MouseButton button, Input::State state) final {
    if (button == Input::MouseButton::Left && state == Input::State::Pressed)
      m_holding = true;
    else if (button == Input::MouseButton::Left && state == Input::State::Released)
      m_holding = false;
  }

  void onMouseMove(float x, float y) final {
    if (m_holding)
      m_admin->getComponent<Components::Physics>(discoLight2).applyAngularImpulse(ml::vec3{y / 20.0f, x / 20.0f, 0.0f});
  }

  void onKey(Input::Key key, Input::State state) final {}

  explicit Demo(int argc = 0, char **argv = nullptr, char **env = nullptr) : Game{argc, argv, env} {
    std::cout << "Created Demo" << '\n';

    m_admin->createSystem<Systems::Physics>();

    auto audio{m_audioManager->createAudioSource("../resources/some.wav")};
    audio->setVolume(100);
    audio->play();

    using (auto entity{m_admin->createEntity()}) {
      // Physics
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
      physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, 0.0f});
      physics.setIsRigid(true);

      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      transform.matrix.setTranslation(ml::vec3{0.0f, -0.0f, 0.0f});

      // Model
      auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/viking_room.obj", "../resources/viking_room.png")};

      test = entity;
    }

    ///////////////////////////////////////
    // Lights
    ///////////////////////////////////////
    using (auto entity{m_admin->createEntity()}) {
      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      transform.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 0.0f});

      // Light
      auto &light{m_admin->createComponent<Components::Light>(entity)};
      light.type  = Components::Light::Type::Point;
      light.color = ml::vec3(1.0f, 1.0f, 1.0f);
      light.size  = 1.0f;
    }

    using (auto entity{m_admin->createEntity()}) {
      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      transform.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 0.0f});

      // Light
      auto &light{m_admin->createComponent<Components::Light>(entity)};
      light.type  = Components::Light::Type::Point;
      light.color = ml::vec3(1.0f, 0.25f, 0.0f) * 3.0f;
      light.size  = 0.3f;
    }

    // using (auto entity{m_admin->createEntity()}) {
    //   // Transform
    //   auto &transform{m_admin->createComponent<Components::Transform>(entity)};
    //   transform.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 0.0f});

    //   // Light
    //   auto &light{m_admin->createComponent<Components::Light>(entity)};
    //   light.type      = Components::Light::Type::Directional;
    //   light.color     = ml::vec3(0.0f, 0.0f, 1.0f);
    //   light.direction = ml::vec3(-1.0f, 0.0f, 0.0f);

    //   // Physics
    //   auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
    //   physics.setIsRigid(true);
    //   discoLight = entity;
    // }

    // using (auto entity{m_admin->createEntity()}) {
    //   // Transform
    //   auto &transform{m_admin->createComponent<Components::Transform>(entity)};
    //   transform.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 0.0f});

    //   // Light
    //   auto &light{m_admin->createComponent<Components::Light>(entity)};
    //   light.type      = Components::Light::Type::Directional;
    //   light.color     = ml::vec3(0.0f, 0.0f, 1.0f);
    //   light.direction = ml::vec3(1.0f, 0.0f, 0.0f);

    //   // Physics
    //   auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
    //   physics.setIsRigid(true);
    //   discoLight2 = entity;
    // }
  }

  [[nodiscard]] bool update(float dt, std::uint64_t) final {
    // m_admin->getComponent<Components::Physics>(discoLight).applyAngularImpulse(ml::vec3{0.0f, 0.0f, 10.0f * dt});
    // m_admin->getComponent<Components::Physics>(discoLight2).applyAngularImpulse(ml::vec3{0.0f, 0.0f, 10.0f * dt});
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
