#pragma once

#include "Entity.hpp"
#include "TypeId.hpp"
#include "ClassId.hpp"
#include "ISystem.hpp"

#include <iostream>
#include <vector>
#include <utility>
#include <functional>
#include <cstddef>
#include <algorithm>
#include <array>
#include <map>

namespace ECS {
  class Admin;
}

class ECS::Admin {
public:
  using EntityArray         = std::vector<Entity>;
  using EntityIndex         = std::uint64_t;
  using EntityIndexArray    = std::vector<EntityIndex>;
  using EntityIterator      = EntityArray::iterator;
  using EntityIteratorArray = std::vector<EntityIterator>;
  using ComponentType       = void *;
  using ComponentDestructor = std::function<void(void *)>;
  using ComponentPool       = std::pair<std::vector<ComponentType>, ComponentDestructor>;
  using ComponentPoolArray  = std::vector<ComponentPool>;
  using SystemIdGenerator   = ClassId<ISystem>;
  using SystemKey           = std::size_t;
  using SystemArray         = std::map<SystemKey, std::unique_ptr<ISystem>>;
  using SystemIterator      = SystemArray::iterator;

private:
  EntityArray        m_entities{};
  EntityIndexArray   m_deadEntities{};
  ComponentPoolArray m_componentPools{};
  SystemArray        m_systems{};
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-private-field"
  std::size_t m_maxId{0};
#pragma clang diagnostic pop

public:
  virtual ~Admin();

  [[nodiscard]] EntityIndex  createEntity();
  void                       destroyEntity(EntityIndex entity);
  [[nodiscard]] EntityArray &getEntities();

  [[nodiscard]] inline Entity &getEntity(EntityIndex entity) {
    if (entity >= m_entities.size())
      throw std::runtime_error{"Entity index is invalid"};

    return m_entities[entity];
  }

  [[nodiscard]] inline decltype(auto) getEntityIterator(EntityIndex entity) {
    if (entity >= m_entities.size())
      throw std::runtime_error{"Entity index is invalid"};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
    return m_entities.begin() + entity;
#pragma clang diagnostic pop
  }

  template <typename... Ts>
  [[nodiscard]] std::vector<std::tuple<EntityIndex, Ts &...>> getEntitiesWithComponents();

  template <typename T, typename... Ts>
  T &createComponent(EntityIndex entity, Ts &&...ts);

  template <typename T>
  void destroyComponent(EntityIndex entity);

  template <typename T>
  [[nodiscard]] bool hasComponent(EntityIndex entity);

  template <typename... Ts>
  [[nodiscard]] bool hasComponents(EntityIndex entity);

  template <typename T>
  [[nodiscard]] T &getComponent(EntityIndex entity);

  template <typename... Ts>
  [[nodiscard]] std::tuple<Ts &...> getComponents(EntityIndex entity);

  template <class T, typename... Ts>
  SystemIterator createSystem(Ts &&...args);

  template <class T>
  [[nodiscard]] T &getSystem();

  template <class T>
  void destroySystem();

  template <class T>
  [[nodiscard]] bool hasSystem();

  void cacheSystems();
  void updateSystems(float deltatime, std::uint64_t tickCount);
};

inline ECS::Admin::~Admin() {
  for (const auto &[pool, deleter] : m_componentPools) {
    if (!deleter)
      continue;

    for (const auto &component : pool)
      deleter(component);
  }
}

inline ECS::Admin::EntityIndex ECS::Admin::createEntity() {
  if (!m_deadEntities.empty()) {
    auto it{m_deadEntities.begin()};
    auto val{*it};
    m_deadEntities.erase(it);
    return val;
  }

  auto index{m_entities.size()};
  m_entities.emplace_back(0);
  return index;
}

inline void ECS::Admin::destroyEntity(EntityIndex entity) {
  // TODO: Check if entity is already dead
  // *entity = 0;
  getEntity(entity) = 0;
  m_deadEntities.emplace_back(entity);
}

inline ECS::Admin::EntityArray &ECS::Admin::getEntities() {
  return m_entities;
}

template <typename... Ts>
inline std::vector<std::tuple<ECS::Admin::EntityIndex, Ts &...>> ECS::Admin::getEntitiesWithComponents() {
  decltype(getEntitiesWithComponents<Ts...>()) entities{};
  for (std::uint64_t i{0}; i < m_entities.size(); ++i)
    if (hasComponents<Ts...>(i))
      entities.emplace_back(i, getComponent<Ts>(i)...);

  return entities;
}

template <typename T, typename... Ts>
inline T &ECS::Admin::createComponent(EntityIndex entity, Ts &&...ts) {
  auto &&[id, bitId]{TypeId::info<T>()};
  auto &bitfield{getEntity(entity)};

  if (id >= m_maxId) {
    m_componentPools.resize(id + 1);
    m_componentPools[id].second = [](void *ptr) {
      delete static_cast<T *>(ptr);
    };
    m_maxId = id + 1;
  }

  auto &&[pool, deleter]{m_componentPools[id]};
  if (pool.size() <= entity)
    pool.resize(entity + 1);

  auto &component{pool[entity]};
  if (component != nullptr)
    new (component) T{std::forward<Ts>(ts)...};
  else
    component = new T{std::forward<Ts>(ts)...};

  bitfield |= bitId;
  return *static_cast<T *>(component);
}

template <typename T>
inline void ECS::Admin::destroyComponent(EntityIndex entity) {
  auto &&[id, bitId]{TypeId::info<T>()};
  auto &bitfield{getEntity(entity)};
  bitfield &= ~bitId;
}

template <typename T>
inline bool ECS::Admin::hasComponent(EntityIndex entity) {
  auto &bitfield{getEntity(entity)};
  auto  bitId{TypeId::info<T>().second};
  return ((bitfield & bitId) == bitId);
}

template <typename... Ts>
inline bool ECS::Admin::hasComponents(EntityIndex entity) {
  return (hasComponent<Ts>(entity) * ...);
}

template <typename T>
inline T &ECS::Admin::getComponent(EntityIndex entity) {
  if (!hasComponent<T>(entity))
    throw std::runtime_error{std::string{"Couldn't get component "} + typeid(T).name()};

  auto &&[id, bitId]{TypeId::info<T>()};
  auto &&[pool, deleter]{m_componentPools[id]};
  return *static_cast<T *>(pool[entity]);
}

template <typename... Ts>
inline std::tuple<Ts &...> ECS::Admin::getComponents(EntityIndex entity) {
  std::tuple<Ts &...> tuple{getComponent<Ts>(entity)...};
  return tuple;
}

template <class T, typename... Ts>
inline ECS::Admin::SystemIterator ECS::Admin::createSystem(Ts &&...ts) {
  const auto id{SystemIdGenerator::info<T>()};
  auto &&[it, success]{m_systems.emplace(id, std::make_unique<T>(*this, std::forward<Ts>(ts)...))};
  if (!success || (*it).second == nullptr)
    throw std::runtime_error{std::string{"Couldn't create system"} + typeid(T).name()};
  return it;
}

template <class T>
T &ECS::Admin::getSystem() {
  const auto id{SystemIdGenerator::info<T>()};
  if (!m_systems.contains(id))
    throw std::runtime_error{"Couldn't get system, not found"};
  return reinterpret_cast<T &>(*(m_systems[id].get()));
}

template <class T>
inline bool ECS::Admin::hasSystem() {
  const auto id{SystemIdGenerator::info<T>()};
  return m_systems.find(id) != m_systems.end();
}

template <class T>
inline void ECS::Admin::destroySystem() {
  const auto id{SystemIdGenerator::info<T>()};
  auto       it{m_systems.find(id)};
  if (it != m_systems.end())
    m_systems.erase(it);
}

inline void ECS::Admin::cacheSystems() {
  std::for_each(m_systems.begin(), m_systems.end(), [](auto &pair) {
    pair.second->cache();
  });
}

inline void ECS::Admin::updateSystems(float deltatime, std::uint64_t tickCount) {
  std::for_each(m_systems.begin(), m_systems.end(), [deltatime, tickCount](auto &pair) {
    pair.second->update(deltatime, tickCount);
  });
}
