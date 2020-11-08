#pragma once

#include "ISystem.hpp"
#include "Admin.hpp"

#include <vector>

namespace ECS {
  template <typename... Ts>
  class System;
}

template <typename... Ts>
class ECS::System : public ECS::ISystem {
public:
  using CachedItems = std::vector<std::tuple<ECS::Admin::EntityIndex, Ts &...>>;

private:
  CachedItems m_cachedItems{};

protected:
  Admin &m_admin;

public:
  explicit System(decltype(m_admin) admin);
  ~System() override = default;

  [[nodiscard]] CachedItems &getItems();
  void                       cache() final;
};

template <typename... Ts>
ECS::System<Ts...>::System(decltype(m_admin) admin) : m_admin{admin} {}

template <typename... Ts>
typename ECS::System<Ts...>::CachedItems &ECS::System<Ts...>::getItems() {
  return m_cachedItems;
}

template <typename... Ts>
void ECS::System<Ts...>::cache() {
  m_cachedItems = std::move(m_admin.getEntitiesWithComponents<Ts...>());
}
