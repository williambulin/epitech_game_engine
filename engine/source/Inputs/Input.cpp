
#include "Inputs/Input.hpp"
#include <iostream>
#include <conio.h>
#ifndef UNICODE
#define UNICODE
#endif

LRESULT CALLBACK EventHandler(HWND, unsigned, WPARAM, LPARAM);

Input::Input() {
  _mouse           = new Mouse();
  WNDCLASS wc      = {};
  wc.lpfnWndProc   = (WNDPROC)EventHandler;
  wc.style         = CS_DBLCLKS;
  wc.lpszClassName = "SimpleEngine Class";
  wc.hInstance     = GetModuleHandle(0);
  if (!RegisterClass(&wc))
    exit(0);
  _dummyHWND = ::CreateWindowA("SimpleEngine Class", "SimpleEngine", WS_VISIBLE, 0, 0, 100, 100, 0, 0, 0, this);
  ::SetWindowTextA(_dummyHWND, "Dummy Window!");

  Log log{"InputsReadyToBeUsed"};
  log->log.Info("Inputs are ready to be used");
  if (_dummyHWND == NULL) {
    Log log{"InputsCantBeUsed"};
    log->log.Critical("Inputs can't be used, the dummy window crashed");
    exit(0);
  }
}

auto Input::getKeypressed() {
  return _keys;
}

void Input::addKeypressed(int key) {
  for (auto it = _keys.begin(); it != _keys.end(); it++)
    if ((*it)->getKey() == key) {
      (*it)->isPressed();
      return;
    }
  _keys.push_back(new Key(key));
}

void Input::releasedKeys(int key) {
  for (auto it = _keys.begin(); it != _keys.end(); it++)
    if ((*it)->getKey() == key) {
      (*it)->isReleased();
      break;
    }
}

bool Input::keyInState(int key, Key::state_e state) {
  for (auto it = _keys.begin(); it != _keys.end(); it++)
    if ((*it)->getState() == Key::state_e::Pressed) {
      if ((*it)->getKey() == key)
        return true;
    } else if ((*it)->getKey() == key && (*it)->getState() == state)
      return true;
  return false;
}

bool Input::combinationKeyStroke(std::vector<int> comb) {
  int record_comb = 0;
  for (auto it2 = comb.begin(); it2 != comb.end(); it2++)
    for (auto it = _keys.begin(); it != _keys.end(); it++)
      if ((*it)->getKey() == *it2 && (*it)->getState() != Key::state_e::Unknow)
        record_comb++;
  if (record_comb == comb.size())
    return true;
  return false;
}

void Input::startTriggeringInput() {
  for (auto it = _keys.begin(); it != _keys.end(); it++) {
    if ((*it)->isDestroy() == true) {
      _keys.erase(it);
      break;
    }
  }
  _mouse->resetState();
  while (PeekMessage(&_msg, _dummyHWND, 0, 0, PM_REMOVE)) {
    TranslateMessage(&_msg);
    DispatchMessage(&_msg);

    unsigned        size = sizeof(RAWINPUT);
    static RAWINPUT raw[sizeof(RAWINPUT)];
    GetRawInputData((HRAWINPUT)_msg.lParam, RID_INPUT, raw, &size, sizeof(RAWINPUTHEADER));

    _mouse->setWheel(GET_WHEEL_DELTA_WPARAM(_msg.wParam));
    if (raw->header.dwType == RIM_TYPEMOUSE)
      _mouse->setMousePosition(raw->data.mouse.lLastX, raw->data.mouse.lLastY);

    switch (_msg.message) {
      case WM_LBUTTONDBLCLK:
        _mouse->stateChanges(Mouse::INPUT_e::Left, Mouse::STATE_e::DBClick);
        break;
      case WM_RBUTTONDBLCLK:
        _mouse->stateChanges(Mouse::INPUT_e::Right, Mouse::STATE_e::DBClick);
        break;
      case WM_LBUTTONDOWN:
        _mouse->stateChanges(Mouse::INPUT_e::Left, Mouse::STATE_e::Pressed);
        break;
      case WM_LBUTTONUP:
        _mouse->stateChanges(Mouse::INPUT_e::Left, Mouse::STATE_e::Released);
        break;
      case WM_RBUTTONDOWN:
        _mouse->stateChanges(Mouse::INPUT_e::Right, Mouse::STATE_e::Pressed);
        break;
      case WM_RBUTTONUP:
        _mouse->stateChanges(Mouse::INPUT_e::Right, Mouse::STATE_e::Released);
        break;
      case WM_MBUTTONDOWN:
        _mouse->stateChanges(Mouse::INPUT_e::Middle, Mouse::STATE_e::Pressed);
        break;
      case WM_MBUTTONUP:
        _mouse->stateChanges(Mouse::INPUT_e::Middle, Mouse::STATE_e::Released);
        break;

      case WM_KEYUP:
        releasedKeys(_msg.wParam);
        break;
      case WM_KEYDOWN:
        switch (_msg.wParam) {
          case 0x1B:
            DestroyWindow(_dummyHWND);
            exit(0);
          default:
            addKeypressed(_msg.wParam);
            break;
        }
        break;
      case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
  }
}

LRESULT CALLBACK EventHandler(HWND hwnd, unsigned event, WPARAM wparam, LPARAM lparam) {
  return DefWindowProc(hwnd, event, wparam, lparam);
}
