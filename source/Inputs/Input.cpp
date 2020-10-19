
#include "Inputs/Input.hpp"
#include <iostream>
#include <conio.h>

Input::Input() {
  _dummyHWND = ::CreateWindowA("STATIC", "dummy", WS_VISIBLE, 0, 0, 100, 100, NULL, NULL, NULL, NULL);
  ::SetWindowTextA(_dummyHWND, "Dummy Window!");
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


bool Input::keyIsPressed(int key, Key::state_e state) {
  for (auto it = _keys.begin(); it != _keys.end(); it++) 
    if ((*it)->getKey() == key && (*it)->getState() == state)
      return true;
  return false;
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
              exit(0);
            default:
              std::cout << _msg.wParam << std::endl;
              addKeypressed(_msg.wParam);
              break;
          }
          break;
        default:
          for (auto it = _keys.begin(); it != _keys.end(); it++) {
            if ((*it)->isDestroy() == true) {
              _keys.erase(it);
              break;
            }
          }
          break;
      }
    }
  }
}
