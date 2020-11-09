#pragma once

#include <utility>
#include <type_traits>
#include <memory>
#include <stdexcept>

namespace ECS {
  template <class B>
  class ClassId;
}

template <class B>
class ECS::ClassId final {
private:
  static inline std::size_t m_count{0};

public:
  using InfoType = decltype(m_count);

public:
  template <class T>
  [[nodiscard]] static const InfoType &info();
};

template <class B>
template <class T>
inline const typename ECS::ClassId<B>::InfoType &ECS::ClassId<B>::info() {
  if (std::is_base_of<T, B>::value)  // derived_from
    throw std::runtime_error{std::string{"Type "} + typeid(B).name() + " does not derivate from " + typeid(T).name()};

  static decltype(info<T>()) info{m_count++};
  return info;
}
