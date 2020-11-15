#pragma once

#include <utility>
#include <memory>
#include <stdexcept>
#include <set>

namespace ECS {
  class TypeId;
}

class ECS::TypeId final {
private:
  // static inline std::size_t m_count{0};
  // static inline std::size_t m_bitcount{0};
  static inline std::set<std::string> test{};

public:
  template <typename T>
  [[nodiscard]] static std::size_t info();
};

template <typename T>
inline std::size_t ECS::TypeId::info() {
  std::string name{typeid(T).name()};
  if (!test.contains(name))
    test.insert(name);
  return std::distance(test.begin(), test.find(name));
}
