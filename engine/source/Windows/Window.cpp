#include "Window.hpp"

#include <iostream>

static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  auto window{reinterpret_cast<Windows::Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA))};

  switch (msg) {
    case WM_DESTROY:
    case WM_CLOSE:
      window->closeWindow();
      break;
    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

bool Windows::Window::isWindowOpen() const noexcept {
  return m_windowOpen;
}

void Windows::Window::closeWindow() {
  // if (m_windowOpen == false)
  //   throw std::runtime_error{"Window is already closed"};

  m_windowOpen = false;
}

auto Windows::Window::getHandle() const noexcept -> HWND {
  return m_hwnd;
}

void Windows::Window::processMessages() const noexcept {
  MSG msg{};
  if (PeekMessage(std::addressof(msg), getHandle(), 0, 0, PM_REMOVE)) {
    TranslateMessage(std::addressof(msg));
    DispatchMessage(std::addressof(msg));
  }
}

Windows::Window::Window() {
  WNDCLASS wndClass{
  .lpfnWndProc   = windowProc,
  .cbClsExtra    = sizeof(std::uintptr_t),
  .hInstance     = GetModuleHandle(nullptr),
  .hIcon         = LoadIcon(nullptr, IDI_APPLICATION),
  .hCursor       = LoadCursor(nullptr, IDC_ARROW),
  .lpszClassName = m_className.c_str(),
  };

  RegisterClass(std::addressof(wndClass));

  m_hwnd = CreateWindowEx(0, m_className.c_str(), m_className.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, GetModuleHandle(nullptr), this);
  if (m_hwnd == nullptr)
    throw std::runtime_error{"Couldn't create Win32 window"};

  SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<std::uintptr_t>(this));

  ShowWindow(getHandle(), 1);
  UpdateWindow(getHandle());
}

Windows::Window::~Window() {
  UnregisterClass(m_className.c_str(), GetModuleHandle(nullptr));

  if (m_hwnd != nullptr)
    DestroyWindow(m_hwnd);
}
