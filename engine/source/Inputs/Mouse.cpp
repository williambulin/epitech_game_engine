
#include "Mouse.hpp"
#include <iostream>

Mouse::Mouse() {}

auto Mouse::getMouse() {
  return _mouse;
}

int Mouse::getWheelDelta() {
  return _mouse.MClick.wheel;
}

void Mouse::setMousePosition(int pos_x, int pos_y) {
  _mouse.pos.x = pos_x;
  _mouse.pos.y = pos_y;
}

void Mouse::setWheel(int wheel) {
  _mouse.MClick.wheel = wheel;
}

void Mouse::stateChanges(Mouse::INPUT_e input, Mouse::STATE_e state) {
  if (input == Mouse::INPUT_e::Right) {
    _mouse.RClick = state;
  } else if (input == Mouse::INPUT_e::Left) {
    _mouse.LClick = state; 
  } else
    _mouse.MClick.state = state;
}

void Mouse::resetState()
{
  _mouse.LClick = Mouse::STATE_e::Unknow;
  _mouse.RClick = Mouse::STATE_e::Unknow;
  _mouse.MClick.state = Mouse::STATE_e::Unknow;
}

Mouse::STATE_e Mouse::getState(Mouse::INPUT_e input) {
  if (input == Mouse::INPUT_e::Right) {
    return _mouse.RClick;
  } else if (input == Mouse::INPUT_e::Left) {
    return _mouse.LClick;
  } else
    return _mouse.MClick.state;
}
