#include "Game.hpp"
#include "Components/Model.hpp"
#include "Components/Movement.hpp"
#include "Components/Physics.hpp"
#include "Components/Light.hpp"
#include "Components/Camera.hpp"
#include "Systems/Physics.hpp"

#include <iostream>
#include <chrono>
#include <thread>

class UserCommand final {
public:
  enum Buttons {
    Reset    = (1 << 0),
    Left     = (1 << 1),
    Right    = (1 << 2),
    Forward  = (1 << 3),
    Backward = (1 << 4),
    Sprint   = (1 << 5),
    Duck     = (1 << 6),
    Jump     = (1 << 7),
  };

public:
  std::uint64_t buttons{};
};

class Gravity final : public ECS::System<Components::Physics> {
public:
  Gravity(ECS::Admin &admin) : System<Components::Physics>{admin} {}
  ~Gravity() final = default;

  void update(float deltatime, std::uint64_t tickCount) final {
    auto &entities{getItems()};
    for (auto &&[entity, physics] : entities)
      if (!physics.getIsRigid())
        physics.applyLinearImpulse(ml::vec3{0.0f, 0.0f, -9.81f * deltatime * 10.0f});
  }
};

class Demo final : public Game {
public:
  UserCommand                            m_userCommand{};
  bool                                   m_holding{false};
  std::optional<ECS::Admin::EntityIndex> m_camera{std::nullopt};
  std::optional<ECS::Admin::EntityIndex> m_obj{std::nullopt};
  std::optional<ECS::Admin::EntityIndex> m_discoLight1{std::nullopt};
  std::optional<ECS::Admin::EntityIndex> m_discoLight2{std::nullopt};

  void onMouseButton(Input::MouseButton button, Input::State state) final {
    if (button == Input::MouseButton::Left && state == Input::State::Pressed)
      m_holding = true;
    else if (button == Input::MouseButton::Left && state == Input::State::Released)
      m_holding = false;
  }

  void onMouseMove(float x, float y) final {
    if (m_holding && m_camera.has_value()) {
      auto &cameraComponent{m_admin->getComponent<Components::Camera>(m_camera.value())};
      cameraComponent.angles += ml::vec3(-y * 0.033f * 9.0f, x * 0.033f * 9.0f, 0.0f);
      while (cameraComponent.angles.y >= 360.0f)
        cameraComponent.angles.y -= 360.0f;
      while (cameraComponent.angles.y < 0.0f)
        cameraComponent.angles.y += 360.0f;
      if (cameraComponent.angles.x < -89.0f)
        cameraComponent.angles.x = -89.0f;
      if (cameraComponent.angles.x > 89.0f)
        cameraComponent.angles.x = 89.0f;
    }
  }

  void onKey(Input::Key key, Input::State state) final {
    switch (key) {
      case Input::Key::W:
        if (state == Input::State::Pressed)
          m_userCommand.buttons |= UserCommand::Buttons::Forward;
        else if (state == Input::State::Released)
          m_userCommand.buttons &= ~UserCommand::Buttons::Forward;
        break;
      case Input::Key::A:
        if (state == Input::State::Pressed)
          m_userCommand.buttons |= UserCommand::Buttons::Left;
        else if (state == Input::State::Released)
          m_userCommand.buttons &= ~UserCommand::Buttons::Left;
        break;
      case Input::Key::S:
        if (state == Input::State::Pressed)
          m_userCommand.buttons |= UserCommand::Buttons::Backward;
        else if (state == Input::State::Released)
          m_userCommand.buttons &= ~UserCommand::Buttons::Backward;
        break;
      case Input::Key::D:
        if (state == Input::State::Pressed)
          m_userCommand.buttons |= UserCommand::Buttons::Right;
        else if (state == Input::State::Released)
          m_userCommand.buttons &= ~UserCommand::Buttons::Right;
        break;
      case Input::Key::LShift:
        if (state == Input::State::Pressed)
          m_userCommand.buttons |= UserCommand::Buttons::Sprint;
        else if (state == Input::State::Released)
          m_userCommand.buttons &= ~UserCommand::Buttons::Sprint;
        break;
      case Input::Key::C:
        if (state == Input::State::Pressed)
          m_userCommand.buttons |= UserCommand::Buttons::Duck;
        else if (state == Input::State::Released)
          m_userCommand.buttons &= ~UserCommand::Buttons::Duck;
        break;
      case Input::Key::Space:
        if (state == Input::State::Pressed)
          m_userCommand.buttons |= UserCommand::Buttons::Jump;
        else if (state == Input::State::Released)
          m_userCommand.buttons &= ~UserCommand::Buttons::Jump;
        break;
    }
  }

  explicit Demo(int argc = 0, char **argv = nullptr, char **env = nullptr) : Game{argc, argv, env} {
    std::cout << "Created Demo" << '\n';

    m_admin->createSystem<Systems::Physics>();
    // m_admin->createSystem<Gravity>();

    auto audio{m_audioManager->createAudioSource("../resources/some.wav")};
    audio->setVolume(100);
    audio->play();

    ///////////////////////////////////////
    // Objects
    ///////////////////////////////////////
    using (auto entity{m_admin->createEntity()}) {
      // Physics
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-50.0f, 0.0f, -50.0f}, ml::vec3{50.0f, 100.0f, 50.0f}))};
      physics.setIsRigid(true);

      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};

      // Model
      auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/untitled2.obj", "../resources/untitled2.png")};
    }

    using (auto entity{m_admin->createEntity()}) {
      // Physics
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
      physics.applyAngularImpulse(ml::vec3{0.0f, 0.0f, 0.0f});
      physics.applyLinearImpulse(ml::vec3{-30.0f, 0.0f, 0.0f});
      physics.setIsRigid(false);

      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      transform.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 55.0f});

      // Model
      auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/3b0b075229e84317a014fb275a5d8dbe.obj", "../resources/backpack.jpg")};

      m_obj = entity;
    }

    using (auto entity{m_admin->createEntity()}) {
      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};

      // Model
      auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/sky.obj", "../resources/sky.png")};
    }

    ///////////////////////////////////////
    // Camera
    ///////////////////////////////////////
    using (auto entity{m_admin->createEntity()}) {
      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      // transform.matrix.setTranslation(ml::vec3{1.72966f, 4.71852f, 4.25082f});
      transform.matrix.setTranslation(ml::vec3{1.72966f, 4.71852f, 54.25082f});

      // Camera
      auto &camera{m_admin->createComponent<Components::Camera>(entity)};
      camera.angles = ml::vec3{-44.847f, 270.009f, 0.0f};

      // Physics
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};

      m_camera = entity;
    }

    ///////////////////////////////////////
    // Lights
    ///////////////////////////////////////
    using (auto entity{m_admin->createEntity()}) {
      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      transform.matrix.setTranslation(ml::vec3{0.0f, 0.0f, 50.0f});

      // Light
      auto &light{m_admin->createComponent<Components::Light>(entity)};
      light.type  = Components::Light::Type::Point;
      light.color = ml::vec3(1.0f, 1.0f, 1.0f);
      light.size  = 1.0f;
    }

    using (auto entity{m_admin->createEntity()}) {
      // Transform
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      transform.matrix.setTranslation(ml::vec3{0.1f, 0.0f, 50.0f});

      // Light
      auto &light{m_admin->createComponent<Components::Light>(entity)};
      light.type  = Components::Light::Type::Point;
      light.color = ml::vec3(0.0f, 1.25f, 0.0f) * 10.0f;
      light.size  = 0.3f;
    }
  }

  [[nodiscard]] bool update(float dt, std::uint64_t) final {
    if (m_camera.has_value()) {
      auto &&[camera, transform, physics]{m_admin->getComponents<Components::Camera, Components::Transform, Components::Physics>(m_camera.value())};

      auto  position{ml::vec3{0.0f, 0.0f, 0.0f}};  // transform.matrix.getTranslation()};
      float speed{(m_userCommand.buttons & UserCommand::Buttons::Sprint) ? 150.0f : 50.0f};
      if (m_userCommand.buttons & UserCommand::Buttons::Forward)
        position += camera.m_front * speed * dt;
      if (m_userCommand.buttons & UserCommand::Buttons::Backward)
        position -= camera.m_front * speed * dt;
      if (m_userCommand.buttons & UserCommand::Buttons::Left)
        position -= camera.m_right * speed * dt;
      if (m_userCommand.buttons & UserCommand::Buttons::Right)
        position += camera.m_right * speed * dt;
      if (m_userCommand.buttons & UserCommand::Buttons::Jump)
        position += camera.m_worldUp * speed * dt;
      if (m_userCommand.buttons & UserCommand::Buttons::Duck)
        position -= camera.m_worldUp * speed * dt;

      physics.applyLinearImpulse(position);

      // transform.matrix.setTranslation(position);
      position          = transform.matrix.getTranslation();
      camera.m_position = position;
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
