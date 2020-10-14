#pragma once

#include "Key.hpp"

#include <array>
#include <iterator>

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

class Input {
public:
private:
  MSG                          _msg;
  HWND                         _dummyHWND;
  std::array<int, 6>           _keypressed;
  std::array<int, 6>::iterator _it;
  Key                         *_keys;

public:
  explicit Input();
  virtual ~Input() = default;
  auto getKeypressed();
  void addKeypressed(int);
  void startTriggeringInput();
  void releasedKeys(int);
};
