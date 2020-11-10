#include "Game.hpp"

#include <memory>

bool Game::start() {
  float         deltatime{0.0f};
  std::uint64_t tickCount{0};
  while (m_window->isWindowOpen()) {
    m_window->processMessages();

    m_admin->cacheSystems();
    m_admin->updateSystems(deltatime, tickCount);

    if (!update(deltatime, tickCount))
      return false;

    // Update the renderer system
    m_renderer->update(deltatime, tickCount);
    ++tickCount;
  }
}

Game::Game(int argc, char **argv, char **env) : ApplicationBase{argc, argv, env} {
  m_window = std::make_unique<Windows::Window>();
  if (m_window == nullptr)
    throw std::runtime_error{"Couldn't create Windows::Window"};

  m_admin = std::make_unique<ECS::Admin>();
  if (m_admin == nullptr)
    throw std::runtime_error{"Couldn't create ECS::Admin"};

  m_renderer = std::make_unique<Vulkan::Renderer>(*(m_admin.get()), *(m_window.get()));
  if (m_renderer == nullptr)
    throw std::runtime_error{"Couldn't create Vulkan::Renderer"};

  m_audioManager = std::make_unique<AudioManager>();
  if (m_audioManager == nullptr)
    throw std::runtime_error{"Couldn't create AudioManager"};

  m_audioManager->startStream();
}

Game::~Game() {
  m_audioManager->stopStream();
  m_admin.reset();
}
