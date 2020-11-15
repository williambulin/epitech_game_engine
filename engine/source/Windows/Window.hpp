#pragma once

#include "Extension/Libraries/WinApi.hpp"
#include "Input/MouseButton.hpp"
#include "Input/Key.hpp"
#include "Input/State.hpp"

#include <stdexcept>
#include <functional>

namespace Windows {
  class Window;
}

class Windows::Window final {
private:
  std::string m_className{"Vulkan"};
  HWND        m_hwnd{nullptr};
  bool        m_windowOpen{true};

public:
  float                                                              m_oldX{0.0f}, m_oldY{0.0f};
  std::function<void(Input::MouseButton button, Input::State state)> onMouseButton{};
  std::function<void(float x, float y)>                              onMouseMove{};
  std::function<void(Input::Key key, Input::State state)>            onKey{};

public:
  [[nodiscard]] bool isWindowOpen() const noexcept;
  void               closeWindow();

  [[nodiscard]] auto getHandle() const noexcept -> HWND;

  void processMessages() const noexcept;

  explicit Window();
  ~Window();
};
