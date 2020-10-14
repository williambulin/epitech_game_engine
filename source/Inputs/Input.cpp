
#include "Inputs/Input.hpp"
#include <iostream>
#include <conio.h>

Input::Input() {
  _keypressed.fill(0);
  _it        = _keypressed.begin();
  _dummyHWND = ::CreateWindowA("STATIC", "dummy", WS_VISIBLE, 0, 0, 100, 100, NULL, NULL, NULL, NULL);
  ::SetWindowTextA(_dummyHWND, "Dummy Window!");

  _mouseDevice    = new Emergence::Client::Input::Device(Emergence::Client::Input::Device::DeviceType::Mouse);
  _keyboardDevice = new Emergence::Client::Input::Device(Emergence::Client::Input::Device::DeviceType::Keyboard);
}

auto Input::getKeypressed() {
  return _keypressed;
}

void Input::addKeypressed(int key) {
  for (auto it = _keypressed.begin(); it != _keypressed.end(); it++)
    if (*it == key)
      return;
  *_it = key;
  for (auto it = _keypressed.begin(); it != _keypressed.end(); it++)
    if (*it == 0) {
      _it = it;
      break;
    }
}

void Input::releasedKeys(int key) {
  for (auto it = _keypressed.begin(); it != _keypressed.end(); it++)
    if (*it == key)
      *it = 0;
  _keyboardDevice->getKey(key);
  for (auto it = _keypressed.begin(); it != _keypressed.end(); it++)
    if (*it == 0) {
      _it = it;
      return;
    }
}

void Input::startTriggeringInput() {
  while (1) {
    while (GetMessage(&_msg, _dummyHWND, 0, 0)) {
      TranslateMessage(&_msg);
      DispatchMessage(&_msg);
      switch (_msg.message) {
        case WM_KEYUP:
          releasedKeys(_msg.wParam);
          break;

        case WM_KEYDOWN:
          switch (_msg.wParam) {
            case 0x1B:
              std::cout << "ESCAPE" << std::endl;
              break;

            case 0x09:
              std::cout << "Tab" << std::endl;

              exit(0);
            default:
              addKeypressed(_msg.wParam);
              break;
          }
          break;
      }
    }
  }
}
