

#include "Core.hpp"
#include "Extension/Language/Keywords.hpp"
#include "Global/ThreadingTest.hpp"

#include <iostream>
#include <memory>

bool Core::start() {
  std::cout << "Application started." << '\n';

  ThreadingTest test(std::make_shared<int>(42));
  test.waitASec();

  return true;
}
