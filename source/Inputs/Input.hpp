#pragma once

#include "Key.hpp"

#include <array>
#include <vector>
#include <iterator>

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

class Input {
public:
private:
  MSG                          _msg;
  HWND                         _dummyHWND;
  std::vector<Key *>           _keys;

public:
  explicit Input();
  virtual ~Input() = default;
  auto getKeypressed();
  void addKeypressed(int);
  void startTriggeringInput();
  void releasedKeys(int);
  bool keyIsPressed(int, Key::state_e);
};
