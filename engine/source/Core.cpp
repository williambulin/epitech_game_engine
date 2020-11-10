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

  std::uint64_t tickCount{0};
  while (window.isWindowOpen()) {
    static auto previous{std::chrono::high_resolution_clock::now()};
    auto        delta{std::chrono::high_resolution_clock::now() - previous};
    previous = std::chrono::high_resolution_clock::now();

    float dt{std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000000.0f};
    // std::cout << dt << " ms\t|\t" << 1.0f / dt << " FPS" << std::string(16, ' ') << '\r';

    window.processMessages();

    admin.cacheSystems();
    admin.updateSystems(dt, tickCount);

    if (!game->update(dt, tickCount))
      return false;

    // Update the renderer system
    renderer.update(dt, tickCount);
    ++tickCount;
  }

  return true;
}
