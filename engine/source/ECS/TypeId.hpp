#pragma once

#include <utility>
#include <memory>
#include <stdexcept>

namespace ECS {
  class TypeId;
}

class ECS::TypeId final {
private:
  static inline std::size_t m_count{0};
  static inline std::size_t m_bitcount{0};

public:
  using InfoType = std::pair<decltype(m_count), decltype(m_bitcount)>;

public:
  template <typename T>
  [[nodiscard]] static const ECS::TypeId::InfoType &info();
};

template <typename T>
inline const ECS::TypeId::InfoType &ECS::TypeId::info() {
  static decltype(info<T>()) info{m_count++, ((m_bitcount == 0) ? (m_bitcount = 1) : (m_bitcount = (m_bitcount + m_bitcount)))};
  return info;
}
