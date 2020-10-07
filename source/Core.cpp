#include "Core.hpp"
#include "Extension/Language/Keywords.hpp"

#include <iostream>

#include "Model/OBJ.hpp"

bool Core::start() {
  std::cout << "Application started." << '\n';
  OBJ test("C:\\Users\\$8LT000-M5BIVN2H17VO\\Documents\\Epitech\\gameengine\\epitech_game_engine\\test.obj");
  return true;
}
