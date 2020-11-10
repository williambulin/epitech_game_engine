#pragma once

#include "Game.hpp"

#include <memory>

std::unique_ptr<Game> loadGame(Vulkan::Renderer &renderer, ECS::Admin &admin, AudioManager &audioManager);
