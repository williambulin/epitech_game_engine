#pragma once

#include <atomic>
#include <ctime>
#include <memory>

class ThreadingTest {
public:
  using Internal	= std::atomic<std::shared_ptr<int>>;

private:
  Internal	m_value{};

public:
  explicit ThreadingTest(std::shared_ptr<int>);
  ThreadingTest(const ThreadingTest &)	= delete;
  virtual ~ThreadingTest()				= default;

  auto operator=(const ThreadingTest &) -> ThreadingTest &	= delete;

  [[nodiscard]] auto getValue() const -> const Internal &;
  void waitASec() const;
};
