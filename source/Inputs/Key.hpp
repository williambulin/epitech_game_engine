#pragma once

class Key {
public:
  typedef enum {
    Unknow,
    Pressed,
    Released,
    Repeated,
  } state_e;

private:
  int     _key;
  state_e _state;

public:
  explicit Key();
  virtual ~Key() = default;
  int  getKey();
  auto getState();
};
