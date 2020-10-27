#pragma once

#include "Key.hpp"
#include "Mouse.hpp"

class Input {
public:
private:
  MSG                _msg;
  HWND               _dummyHWND;
  std::vector<Key *> _keys;
  Mouse *            _mouse;

public:
  explicit Input();
  virtual ~Input() = default;
  auto getKeypressed();
  void addKeypressed(int);
  void startTriggeringInput();
  void releasedKeys(int);
  bool keyInState(int, Key::state_e);
  bool combinationKeyStroke(std::vector<int>);
  };
