#pragma once

#include "Vulkan/Renderer.hpp"
#include "ECS/Admin.hpp"
#include "ECS/System.hpp"
#include "Components/Transform.hpp"
#include "Audio/AudioManager.hpp"
#include "Global/ApplicationBase.hpp"

class Game : public ApplicationBase {
protected:
  std::unique_ptr<Windows::Window>  m_window{nullptr};
  std::unique_ptr<ECS::Admin>       m_admin{nullptr};
  std::unique_ptr<Vulkan::Renderer> m_renderer{nullptr};
  std::unique_ptr<AudioManager>     m_audioManager{nullptr};

public:
  [[nodiscard]] bool start() final;

  [[nodiscard]] virtual bool update(float frametime, std::uint64_t tickCount) = 0;

  explicit Game(int argc = 0, char **argv = nullptr, char **env = nullptr);
  virtual ~Game();
};
