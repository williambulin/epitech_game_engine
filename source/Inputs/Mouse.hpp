#pragma once

#include <array>
#include <vector>
#include <iterator>

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

class Mouse {
public:
  typedef enum class INPUT { Right, Left, Middle } INPUT_e;
  typedef enum class STATE { Unknow, Pressed, Released, DBClick } STATE_e;
  typedef struct {
    STATE_e state;
    int     wheel;
  } MID_t;
  typedef struct {
    int x;
    int y;
  } pos_t;
  typedef struct {
    STATE_e RClick;
    STATE_e LClick;
    MID_t   MClick;
    pos_t   pos;
  } mouse_t;

private:
  UINT    _button;
  mouse_t _mouse;

public:
  explicit Mouse();
  virtual ~Mouse() = default;
  auto getMouse();
  int  getWheelDelta();
  STATE_e getState(INPUT_e);
  void setMousePosition(int, int);
  void setWheel(int);
  void stateChanges(INPUT_e, STATE_e);
  void resetState();
};