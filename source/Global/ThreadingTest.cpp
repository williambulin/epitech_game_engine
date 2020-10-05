#include "ThreadingTest.hpp"
#include "Extension/Language/Keywords.hpp"
#include "Extension/Language/Platform.hpp"

#if defined(OS_WINDOWS)
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <memory>
#include <thread>

ThreadingTest::ThreadingTest(std::shared_ptr<int> value) : m_value(value) {}

auto ThreadingTest::getValue() const -> const Internal & {
  return m_value;
}

void ThreadingTest::waitASec() const {
  auto waitThread = std::async(std::launch::async, []() {
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(1000));
  });
  auto start      = std::chrono::high_resolution_clock::now();
  waitThread.wait();
  auto                                      end        = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double, std::milli> elapsed    = end - start;
  std::cout << "Waited " << elapsed.count() << "milliseconds." << std::endl;
}