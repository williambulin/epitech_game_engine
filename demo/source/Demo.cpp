#include "Game.hpp"
#include "Components/Model.hpp"
#include "Components/Physics.hpp"
#include "Components/Light.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Systems/Gravity.hpp"
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
    Attack1  = (1 << 8),
    Attack2  = (1 << 9),
  };

public:
  std::uint64_t buttons{};
};

class Demo final : public Game {
public:
  UserCommand                            m_userCommand{};
  std::optional<ECS::Admin::EntityIndex> m_camera{std::nullopt};
  std::optional<ECS::Admin::EntityIndex> m_skybox{std::nullopt};
  std::optional<ECS::Admin::EntityIndex> m_target{std::nullopt};
  std::optional<ECS::Admin::EntityIndex> m_block{std::nullopt};
  float                                  m_targetDistance{0.0f};
  std::size_t                            m_count{0};

  std::shared_ptr<AudioSource> m_music{nullptr};
  std::shared_ptr<AudioSource> m_physcannonDrop{nullptr};
  std::shared_ptr<AudioSource> m_physcannonDryFire{nullptr};
  std::shared_ptr<AudioSource> m_physcannonPickup{nullptr};
  std::shared_ptr<AudioSource> m_superphysHoldLoop{nullptr};
  std::shared_ptr<AudioSource> m_superphysLaunch{nullptr};

  void onMouseButton(Input::MouseButton button, Input::State state) final {
    switch (button) {
      case Input::MouseButton::Left:
        if (state == Input::State::Pressed) {
          m_userCommand.buttons |= UserCommand::Buttons::Attack1;
          if (m_camera != std::nullopt && m_target != std::nullopt) {
            auto &physics{m_admin->getComponent<Components::Physics>(m_target.value())};
            auto &camera{m_admin->getComponent<Components::Camera>(m_camera.value())};

            physics.applyLinearImpulse(camera.m_front * 100.0f);

            m_superphysLaunch->play();
            m_superphysHoldLoop->stop();
            m_target = std::nullopt;
          }
        } else if (state == Input::State::Released)
          m_userCommand.buttons &= ~UserCommand::Buttons::Attack1;
        break;
      case Input::MouseButton::Right:
        if (state == Input::State::Pressed)
          m_userCommand.buttons |= UserCommand::Buttons::Attack2;
        else if (state == Input::State::Released) {
          m_userCommand.buttons &= ~UserCommand::Buttons::Attack2;

          if (m_camera.has_value() && m_target == std::nullopt) {
            auto &&[camera, transform, physics]{m_admin->getComponents<Components::Camera, Components::Transform, Components::Physics>(m_camera.value())};

            Ray          ray{camera.m_position, camera.m_front};
            RayCollision rayCollision{};

            if (m_admin->getSystem<Systems::Physics>().RayIntersection(ray, rayCollision)) {
              auto &&[physics2, transform2]{m_admin->getComponents<Components::Physics, Components::Transform>(rayCollision.node)};
              if (!physics2.getIsRigid()) {
                m_target         = rayCollision.node;
                m_targetDistance = (transform2.matrix.getTranslation() - transform.matrix.getTranslation()).length();
                m_physcannonPickup->play();
                m_superphysHoldLoop->play();
              } else
                m_physcannonDryFire->play();
            } else
              m_physcannonDryFire->play();
          } else if (m_target != std::nullopt) {
            m_physcannonDrop->play();
            m_superphysHoldLoop->stop();
            m_target = std::nullopt;
          }
        }
        break;
    }
  }

  void onMouseMove(float x, float y) final {
    if (m_camera.has_value()) {
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
    bool canSpawn{m_admin->getEntitiesWithComponents<Components::Physics>().size() < 18};
    switch (key) {
      case Input::Key::Number1:
        if (state == Input::State::Released && canSpawn) {
          use(auto entity{m_admin->createEntity()}) {
            auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
            auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/cube.obj", "../resources/white.png")};

            m_admin->createComponent<Components::Gravity>(entity);
            auto &transform{m_admin->createComponent<Components::Transform>(entity)};
            transform.matrix.setTranslation(ml::vec3{static_cast<float>(rand() % 180) - 90.0f, 30.0f, static_cast<float>(rand() % 180) - 90.0f});
            auto &light{m_admin->createComponent<Components::Light>(entity)};
            light.type  = Components::Light::Type::Point;
            light.color = ml::vec3{(rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f} * 1.0f;
            light.size  = 10.0f;
          }
        }
        break;
      case Input::Key::Number2:
        if (state == Input::State::Released && canSpawn) {
          use(auto entity{m_admin->createEntity()}) {
            auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<Sphere>(ml::vec3{0.0f, 0.0f, 0.0f}, 1.0f))};
            auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/sphere.obj", "../resources/white.png")};

            m_admin->createComponent<Components::Gravity>(entity);
            auto &transform{m_admin->createComponent<Components::Transform>(entity)};
            transform.matrix.setTranslation(ml::vec3{static_cast<float>(rand() % 180) - 90.0f, 30.0f, static_cast<float>(rand() % 180) - 90.0f});
            auto &light{m_admin->createComponent<Components::Light>(entity)};
            light.type  = Components::Light::Type::Point;
            light.color = ml::vec3{(rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f} * 1.0f;
            light.size  = 10.0f;
          }
        }
        break;
      case Input::Key::Number3:
        if (state == Input::State::Released && canSpawn) {
          use(auto entity{m_admin->createEntity()}) {
            auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<Capsule>(ml::vec3{0.0f, 2.0f, 0.0f}, ml::vec3{0.0f, -2.0f, 0.0f}, 1.0f))};
            auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/capsule.obj", "../resources/white.png")};

            m_admin->createComponent<Components::Gravity>(entity);
            auto &transform{m_admin->createComponent<Components::Transform>(entity)};
            transform.matrix.setTranslation(ml::vec3{static_cast<float>(rand() % 180) - 90.0f, 30.0f, static_cast<float>(rand() % 180) - 90.0f});
            auto &light{m_admin->createComponent<Components::Light>(entity)};
            light.type  = Components::Light::Type::Point;
            light.color = ml::vec3{(rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f} * 1.0f;
            light.size  = 10.0f;
          }
        }
        break;
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
        else if (state == Input::State::Released) {
          m_userCommand.buttons &= ~UserCommand::Buttons::Jump;

          if (m_camera.has_value()) {
            auto &&[camera, transform, physics]{m_admin->getComponents<Components::Camera, Components::Transform, Components::Physics>(m_camera.value())};
            physics.applyLinearImpulse(ml::vec3{0.0f, 30.0f, 0.0f});
          }
        }
        break;
    }
  }

  explicit Demo(int argc = 0, char **argv = nullptr, char **env = nullptr) : Game{argc, argv, env} {
    std::cout << "Created Demo" << '\n';

    m_admin->createSystem<Systems::Physics>();
    m_admin->createSystem<Systems::Gravity>();

    m_music             = m_audioManager->createAudioSource("../resources/music/17. Triage at Dawn.flac");
    m_physcannonDrop    = m_audioManager->createAudioSource("../resources/sounds/physcannon_drop.wav");
    m_physcannonDryFire = m_audioManager->createAudioSource("../resources/sounds/physcannon_dryfire.wav");
    m_physcannonPickup  = m_audioManager->createAudioSource("../resources/sounds/physcannon_pickup.wav");
    m_superphysHoldLoop = m_audioManager->createAudioSource("../resources/sounds/superphys_hold_loop.wav");
    m_superphysLaunch   = m_audioManager->createAudioSource("../resources/sounds/superphys_launch1.wav");

    m_superphysHoldLoop->m_loop = true;
    m_audioManager->setAudioGroupVolume(100, "Master");

    m_music->m_loop = true;
    m_music->play();

    ///////////////////////////////////////
    // Camera
    ///////////////////////////////////////
    use(auto entity{m_admin->createEntity()}) {
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<Capsule>(ml::vec3{0.0, 2.0f, 0.0f}, ml::vec3{0.0f, -2.0f, 0.0f}, 0.5f))};
      m_admin->createComponent<Components::Gravity>(entity);
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      transform.matrix.setTranslation(ml::vec3{1.72966f, 40.71852f, 4.25082f});
      auto &camera{m_admin->createComponent<Components::Camera>(entity)};
      camera.angles = ml::vec3{-44.847f, 270.009f, 0.0f};
      m_camera      = entity;
    }

    ///////////////////////////////////////
    // Objects
    ///////////////////////////////////////
    use(auto entity{m_admin->createEntity()}) {
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-25.0f, 0.0f, -25.0f}, ml::vec3{25.0f, 20.5f, 25.0f}))};
      physics.setIsRigid(true);
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/gm_construct_grass.obj", "../resources/grass.png")};
    }

    use(auto entity{m_admin->createEntity()}) {
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-200.0f, 0.0f, -200.0f}, ml::vec3{200.0f, 20.0f, 200.0f}))};
      physics.setIsRigid(true);
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/gm_construct_block.obj", "../resources/untitled2.png")};
      m_block = entity;
    }

    for (std::size_t i{0}; i < 10; ++i) {
      use(auto entity{m_admin->createEntity()}) {
        switch (rand() % 2) {
          case 0: {
            auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-1.0f, -1.0f, -1.0f}, ml::vec3{1.0f, 1.0f, 1.0f}))};
            auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/cube.obj", "../resources/white.png")};
            break;
          }
          case 1: {
            auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<Sphere>(ml::vec3{0.0f, 0.0f, 0.0f}, 1.0f))};
            auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/sphere.obj", "../resources/white.png")};
            break;
          }
          case 2: {
            auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<Capsule>(ml::vec3{0.0f, 2.0f, 0.0f}, ml::vec3{0.0f, -2.0f, 0.0f}, 1.0f))};
            auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/capsule.obj", "../resources/white.png")};
            break;
          }
        }
        m_admin->createComponent<Components::Gravity>(entity);
        auto &transform{m_admin->createComponent<Components::Transform>(entity)};
        transform.matrix.setTranslation(ml::vec3{static_cast<float>(rand() % 180) - 90.0f, 30.0f, static_cast<float>(rand() % 180) - 90.0f});
        auto &light{m_admin->createComponent<Components::Light>(entity)};
        light.type  = Components::Light::Type::Point;
        light.color = ml::vec3{(rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f} * 1.0f;
        light.size  = 10.0f;
      }
    }

    use(auto entity{m_admin->createEntity()}) {
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      auto &model{m_admin->createComponent<Components::Model>(entity, *(m_renderer.get()), "../resources/sky.obj", "../resources/sky.png", ml::vec3{10.0f, 10.0f, 10.0f})};
      auto &physics{m_admin->createComponent<Components::Physics>(entity, std::make_unique<AABB>(ml::vec3{-0.1, -0.1 + 1000.0f, -0.1}, ml::vec3{0.1, 0.1 + 1000.0f, 0.1}))};
      physics.setIsRigid(true);
      m_skybox = entity;
    }

    ///////////////////////////////////////
    // Lights
    ///////////////////////////////////////
    use(auto entity{m_admin->createEntity()}) {
      auto &transform{m_admin->createComponent<Components::Transform>(entity)};
      auto &light{m_admin->createComponent<Components::Light>(entity)};
      light.type  = Components::Light::Type::Ambient;
      light.color = ml::vec3{0.05f, 0.05f, 0.05f};
    }
  }

  [[nodiscard]] bool update(float dt, std::uint64_t) final {
    ml::vec3 cameraPosition{0.0f, 0.0f, 0.0f};

    auto        lightEntities{m_admin->getEntitiesWithComponents<Components::Light, Components::Transform>()};
    std::size_t count{0};
    for (auto &&[entities, light, transform] : lightEntities) {
      auto pos{transform.matrix.getTranslation()};
      if (pos.x > -25.0f && pos.x < 25.0f && pos.z > -25.0f && pos.z < 25.0f)
        ++count;
    }

    if (count >= lightEntities.size())
      return false;

    if (m_camera.has_value()) {
      auto &&[camera, transform, physics]{m_admin->getComponents<Components::Camera, Components::Transform, Components::Physics>(m_camera.value())};

      auto  position{ml::vec3{0.0f, 0.0f, 0.0f}};
      float speed{(m_userCommand.buttons & UserCommand::Buttons::Sprint) ? 50.0f : 25.0f};
      if (m_userCommand.buttons & UserCommand::Buttons::Forward)
        position += camera.m_front * speed * dt;
      if (m_userCommand.buttons & UserCommand::Buttons::Backward)
        position -= camera.m_front * speed * dt;
      if (m_userCommand.buttons & UserCommand::Buttons::Left)
        position -= camera.m_right * speed * dt;
      if (m_userCommand.buttons & UserCommand::Buttons::Right)
        position += camera.m_right * speed * dt;
      // if (m_userCommand.buttons & UserCommand::Buttons::Jump)
      //   position += camera.m_worldUp * speed * dt;
      // if (m_userCommand.buttons & UserCommand::Buttons::Duck)
      //   position -= camera.m_worldUp * speed * dt;

      physics.applyLinearImpulse(position);

      if (m_target.has_value()) {
        auto &&[transform2, physics2]{m_admin->getComponents<Components::Transform, Components::Physics>(m_target.value())};
        ml::vec3 newPosition{transform.matrix.getTranslation() + (camera.m_front * 4.0f)};
        ml::vec3 newDirection{newPosition - transform2.matrix.getTranslation()};
        physics2.applyLinearImpulse(physics2.getLinearVelocity() * -1.0f);
        physics2.applyLinearImpulse(newDirection * dt * 2000.0f);
      }

      position          = transform.matrix.getTranslation();
      camera.m_position = position;
      cameraPosition    = camera.m_position;
    }

    if (m_skybox.has_value()) {
      auto &transform{m_admin->getComponent<Components::Transform>(m_skybox.value())};
      auto &physics{m_admin->getComponent<Components::Physics>(m_skybox.value())};
      transform.matrix.setTranslation(cameraPosition);
      physics.applyAngularImpulse(ml::vec3{0.0f, 0.05f * dt, 0.0f});
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
