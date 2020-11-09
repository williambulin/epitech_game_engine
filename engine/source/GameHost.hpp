#pragma once

#include "GameHostLoader.hpp"

#if defined HOST_GAME
using HostGame = HOST_GAME;
#else
using HostGame = Game;
#endif

std::unique_ptr<Game> loadGame(Vulkan::Renderer &renderer, ECS::Admin &admin) {
  return std::make_unique<HostGame>(renderer, admin);
}
