#pragma once

#include <stdexcept>

#include "Extension/Libraries/WinApi.hpp"

namespace Windows {
  class Window;
}

class Windows::Window final {
private:
  std::string m_className{"Vulkan"};
  HWND        m_hwnd{nullptr};
  bool        m_windowOpen{true};

public:
  [[nodiscard]] bool isWindowOpen() const noexcept;
  void               closeWindow();

  [[nodiscard]] auto getHandle() const noexcept -> HWND;

  void processMessages() const noexcept;

  explicit Window();
  ~Window();
};
