#include "Key.hpp"
#include <iostream>

Key::Key(int key = 0) : _start{std::clock()}, _released{0}, _click{0}, _state{Key::state_e::Pressed}, _key{key} {}

int Key::getKey() {
  return _key;
}

Key::state_e Key::getState() {
  return _state;
}

void Key::setState(state_e state) {
  _state = state;
}

void Key::isReleased() {
  _released = std::clock();
  _state = Key::state_e::Released;
}

void Key::isPressed() {
  if (_state == Key::state_e::Released || _state == Key::state_e::Unknow)
    _state = Key::state_e::Repeated;
  _start = std::clock();
}

bool Key::isDestroy() {
  if ((1000.0 * (std::clock() - _start) / CLOCKS_PER_SEC) > 0.4 && _state == Key::state_e::Pressed) {
    _state = Key::state_e::Hold;
  } else if (_state == Key::state_e::Released) {
    _state = Key::state_e::Unknow;
  } else if ((_state == Key::state_e::Unknow) && (1000.0 * (std::clock() - _released) / CLOCKS_PER_SEC) > 0.3)
    return true;
  else
    return false;
}
