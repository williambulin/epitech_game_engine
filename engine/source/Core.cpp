#include "Core.hpp"
#include "Vulkan/Renderer.hpp"
#include "ECS/Admin.hpp"
#include "GameHostLoader.hpp"
#include "Audio/AudioManager.hpp"

#include <iostream>
#include <chrono>

bool Core::start() {
  Windows::Window  window{};
  auto             admin{std::make_unique<ECS::Admin>()};
  Vulkan::Renderer renderer{*(admin.get()), window};
  AudioManager     audioManager{};
  auto             game{loadGame(renderer, *(admin.get()), audioManager)};

  audioManager.startStream();

  float         deltatime{0.0f};
  std::uint64_t tickCount{0};
  while (window.isWindowOpen()) {
    window.processMessages();

    admin->cacheSystems();
    admin->updateSystems(deltatime, tickCount);

    if (!game->update(deltatime, tickCount))
      return false;

    // Update the renderer system (TODO: Merge as ECS::System)
    renderer.update(deltatime, tickCount);
    ++tickCount;
  }

  audioManager.stopStream();

  game.reset();
  admin.reset();
  return true;
}
