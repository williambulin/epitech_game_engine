#include "Window.hpp"

#include <iostream>
#include <strsafe.h>
#include <hidusage.h>
#include <Windowsx.h>

static LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  auto window{reinterpret_cast<Windows::Window *>(GetWindowLongPtr(hwnd, GWLP_USERDATA))};

  switch (msg) {
      // case WM_INPUT: {
      //   UINT            rawInputSize{sizeof(RAWINPUT)};
      //   static RAWINPUT raw{};  // TODO: Unique ptr
      //   if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, reinterpret_cast<LPBYTE>(std::addressof(raw)), std::addressof(rawInputSize), sizeof(RAWINPUTHEADER)) != rawInputSize)
      //     break;

      //   if (raw.header.dwType == RIM_TYPEMOUSE) {
      //     if (window->onMouseMove != nullptr)
      //       window->onMouseMove(static_cast<float>(raw.data.mouse.lLastX), static_cast<float>(raw.data.mouse.lLastY));
      //   }
      //   break;
      // }
    case WM_LBUTTONDOWN:
      if (window->onMouseButton != nullptr)
        window->onMouseButton(Input::MouseButton::Left, Input::State::Pressed);
      break;
    case WM_LBUTTONUP:
      if (window->onMouseButton != nullptr)
        window->onMouseButton(Input::MouseButton::Left, Input::State::Released);
      break;
    case WM_MBUTTONDOWN:
      if (window->onMouseButton != nullptr)
        window->onMouseButton(Input::MouseButton::Middle, Input::State::Pressed);
      break;
    case WM_MBUTTONUP:
      if (window->onMouseButton != nullptr)
        window->onMouseButton(Input::MouseButton::Middle, Input::State::Released);
      break;
    case WM_RBUTTONDOWN:
      if (window->onMouseButton != nullptr)
        window->onMouseButton(Input::MouseButton::Right, Input::State::Pressed);
      break;
    case WM_RBUTTONUP:
      if (window->onMouseButton != nullptr)
        window->onMouseButton(Input::MouseButton::Right, Input::State::Released);
      break;
    case WM_MOUSEMOVE: {
      float newX{static_cast<float>(GET_X_LPARAM(lParam))}, newY{static_cast<float>(GET_Y_LPARAM(lParam))};
      if (window->onMouseMove != nullptr)
        window->onMouseMove(newX - window->m_oldX, newY - window->m_oldY);
      window->m_oldX = newX;
      window->m_oldY = newY;
      break;
    }
    case WM_KEYDOWN:
      window->onKey(static_cast<Input::Key>(wParam), Input::State::Pressed);
      break;
    case WM_KEYUP:
      window->onKey(static_cast<Input::Key>(wParam), Input::State::Released);
      break;
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

  // RAWINPUTDEVICE rawInputDevices[]{
  // {
  // .usUsagePage = HID_USAGE_PAGE_GENERIC,
  // .usUsage     = HID_USAGE_GENERIC_MOUSE,  // Mouse
  // .dwFlags     = RIDEV_INPUTSINK,          // RIDEV_NOLEGACY,
  // .hwndTarget  = getHandle(),
  // },
  // {
  // .usUsagePage = HID_USAGE_PAGE_GENERIC,
  // .usUsage     = HID_USAGE_GENERIC_KEYBOARD,  // Keyboard
  // .dwFlags     = RIDEV_INPUTSINK,             // RIDEV_NOLEGACY,
  // .hwndTarget  = getHandle(),
  // },
  // };

  // if (!RegisterRawInputDevices(rawInputDevices, sizeof(rawInputDevices) / sizeof(RAWINPUTDEVICE), sizeof(RAWINPUTDEVICE)))
  //   throw std::runtime_error{"Couldn't register raw input devices for keyboard and mouse"};
}

Windows::Window::~Window() {
  UnregisterClass(m_className.c_str(), GetModuleHandle(nullptr));

  if (m_hwnd != nullptr)
    DestroyWindow(m_hwnd);
}
