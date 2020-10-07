
#include "Inputs/Input.hpp"
#include <iostream>
#include <conio.h>

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

Input::Input()
{
  _combo.alt = false;
  _combo.ctrl = false;
  _combo.shift = false;
  _combo.dir   = IDLE;
  _combo.key   = -1;
}

auto Input::getKeypressed()
{
  return _combo;
}


void Input::setMetaCombination() 
{
  _combo.shift = false;
  _combo.alt = false;
  _combo.ctrl = false;

  if (GetKeyState(VK_SHIFT) & 0x8000)
    _combo.shift = true;
  
  if (GetKeyState(VK_MENU) & 0x8000)
    _combo.alt = true;
  
  if (GetKeyState(VK_CONTROL) & 0x8000)
    _combo.ctrl = true;

}


void Input::setArrows()
{
  _combo.dir = IDLE;
  if (GetAsyncKeyState(VK_UP))
    _combo.dir = UP;
  else if (GetAsyncKeyState(VK_DOWN))
    _combo.dir = DOWN;
  else if (GetAsyncKeyState(VK_LEFT))
    _combo.dir = LEFT;
  else if (GetAsyncKeyState(VK_RIGHT))
    _combo.dir = RIGHT;
}


void Input::startTriggeringInput()
{
  int key = 0;

  while (1) {
    setMetaCombination();
    setArrows();
    if (kbhit())
      if (key = getch() != 27)
        _combo.key = key;
      else
        break;
    else
      _combo.key = -1;
  }
}
