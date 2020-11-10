#pragma once

#include "Vulkan/Renderer.hpp"
#include "ECS/Admin.hpp"
#include "ECS/System.hpp"
#include "Components/Transform.hpp"
#include "Audio/AudioManager.hpp"

class Game {
protected:
  Vulkan::Renderer &m_renderer;
  ECS::Admin &      m_admin;
  AudioManager &    m_audioManager;

public:
  [[nodiscard]] virtual bool update(float frametime, std::uint64_t tickCount) = 0;

  explicit Game(Vulkan::Renderer &renderer, ECS::Admin &admin, AudioManager &audioManager) : m_renderer{renderer}, m_admin{admin}, m_audioManager{audioManager} {}
  virtual ~Game() = default;
};
