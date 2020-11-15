#pragma once

#include "Extension/Language/Library.hpp"

#include <utility>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <ranges>

namespace ECS {
  class SystemIdRegister;
}

class ECS::SystemIdRegister final {
private:
  DLLATTRIB static std::vector<std::string> m_list;

public:
  template <typename T>
  [[nodiscard]] static std::size_t info();
};

template <typename T>
inline std::size_t ECS::SystemIdRegister::info() {
  static std::string name{typeid(T).name()};
  static std::size_t cachedResult{0};
  static bool        hasCached{false};

  if (!hasCached) {
    if (std::find(m_list.begin(), m_list.end(), name) == m_list.end())
      m_list.emplace_back(name);
    cachedResult = std::distance(m_list.begin(), std::find(m_list.begin(), m_list.end(), name));
    hasCached    = true;
  }
  return cachedResult;
}
