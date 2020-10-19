#pragma once

#include "KeyName.hpp"
#include <chrono>
#include <ctime>

class Key {
public:
  typedef enum class state {
    Unknow   = 0,
    Pressed  = 1,
    Released = 2,
    Repeated = 3,
    Hold     = 4,
  } state_e;

private:
  std::clock_t _start;
  std::clock_t _released;
  std::clock_t _click;
  state_e      _state;
  int          _key;


public:
  explicit Key(int);
  virtual ~Key() = default;
  int     getKey();
  state_e getState();
  void    setState(state_e);
  void    isReleased();
  void    isPressed();
  bool    isDestroy();
};
