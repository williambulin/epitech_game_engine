#pragma once

#include "Vulkan/Renderer.hpp"
#include "ECS/Admin.hpp"
#include "ECS/System.hpp"
#include "Components/Transform.hpp"

class Game {
protected:
  Vulkan::Renderer &m_renderer;
  ECS::Admin &      m_admin;

public:
  [[nodiscard]] virtual bool update(float frametime, std::uint64_t tickCount) = 0;

  explicit Game(Vulkan::Renderer &renderer, ECS::Admin &admin) : m_renderer{renderer}, m_admin{admin} {}
  virtual ~Game() = default;
};
