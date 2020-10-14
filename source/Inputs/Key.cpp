#include "Key.hpp"

Key::Key() {
  _key   = 0;
  _state = Unknow;
}

int Key::getKey() {
  return _key;
}

auto Key::getState() {
  return _state;
}
