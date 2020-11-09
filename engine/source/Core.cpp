#include "Core.hpp"
#include "Vulkan/Renderer.hpp"
#include "ECS/Admin.hpp"
#include "GameHostLoader.hpp"

#include <iostream>
#include <chrono>

bool Core::start() {
  ECS::Admin       admin{};
  Windows::Window  window{};
  Vulkan::Renderer renderer{admin, window};
  auto             game{loadGame(renderer, admin)};

  float         deltatime{0.0f};
  std::uint64_t tickCount{0};
  while (window.isWindowOpen()) {
    window.processMessages();

    admin.cacheSystems();
    admin.updateSystems(deltatime, tickCount);

    if (!game->update(deltatime, tickCount))
      return false;

    // Update the renderer system (TODO: Merge as ECS::System)
    renderer.update(deltatime, tickCount);
    ++tickCount;
  }

  return true;
}
