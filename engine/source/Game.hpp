#pragma once

#include "Extension/Language/Library.hpp"
#include "Vulkan/Renderer.hpp"
#include "ECS/Admin.hpp"
#include "ECS/System.hpp"
#include "Components/Transform.hpp"
#include "Audio/AudioManager.hpp"
#include "Global/ApplicationBase.hpp"

#include <atomic>

class Game : public ApplicationBase {
private:
  std::vector<std::tuple<Input::MouseButton, Input::State>> m_mouseButtonPool{};
  std::vector<std::tuple<float, float>>                     m_mouseMovePool{};
  std::vector<std::tuple<Input::Key, Input::State>>         m_keyPool{};

protected:
  std::unique_ptr<Windows::Window>  m_window{nullptr};
  std::unique_ptr<ECS::Admin>       m_admin{nullptr};
  std::unique_ptr<Vulkan::Renderer> m_renderer{nullptr};
  std::unique_ptr<AudioManager>     m_audioManager{nullptr};

  virtual void onMouseButton(Input::MouseButton button, Input::State state) = 0;
  virtual void onMouseMove(float x, float y)                                = 0;
  virtual void onKey(Input::Key key, Input::State state)                    = 0;

public:
  [[nodiscard]] DLLATTRIB bool start() final;

  [[nodiscard]] virtual bool update(float frametime, std::uint64_t tickCount) = 0;

  DLLATTRIB explicit Game(int argc = 0, char **argv = nullptr, char **env = nullptr);
  DLLATTRIB ~Game() override;
};
