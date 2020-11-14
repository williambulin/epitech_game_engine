#pragma once

#include "Extension/Language/Library.hpp"
#include "Vulkan/Renderer.hpp"
#include "ECS/Admin.hpp"
#include "ECS/System.hpp"
#include "Components/Transform.hpp"
#include "Global/ApplicationBase.hpp"

class Game : public ApplicationBase {
protected:
  std::unique_ptr<Windows::Window>  m_window{nullptr};
  std::unique_ptr<ECS::Admin>       m_admin{nullptr};
  std::unique_ptr<Vulkan::Renderer> m_renderer{nullptr};

public:
  [[nodiscard]] DLLATTRIB bool start() final;

  [[nodiscard]] virtual bool update(float frametime, std::uint64_t tickCount) = 0;

  DLLATTRIB explicit Game(int argc = 0, char **argv = nullptr, char **env = nullptr);
  DLLATTRIB ~Game() override;
};
