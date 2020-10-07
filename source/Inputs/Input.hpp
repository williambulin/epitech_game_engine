#pragma once

#include <vector>

class Input {
public:
  typedef enum {
      IDLE,
      UP,
      DOWN,
      LEFT,
      RIGHT
  } _arrows_t;
  
  typedef struct {
    bool ctrl;
    bool alt;
    bool shift;
    _arrows_t dir;
    int  key;
  } _combination_t; 

private:
  _combination_t _combo;
	
public:
  explicit      Input();
  virtual       ~Input() = default;
  auto          getKeypressed();
  void          startTriggeringInput();
  void          setMetaCombination();
  void          setArrows();
};
