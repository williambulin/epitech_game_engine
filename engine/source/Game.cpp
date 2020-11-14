#include "Game.hpp"

#include <memory>
#include <chrono>

bool Game::start() {
  std::uint64_t tickCount{0};
  while (m_window->isWindowOpen()) {
    static auto previous{std::chrono::high_resolution_clock::now()};
    auto        delta{std::chrono::high_resolution_clock::now() - previous};
    previous = std::chrono::high_resolution_clock::now();

    float dt{std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000000.0f};
    // std::cout << dt << " ms\t|\t" << 1.0f / dt << " FPS" << std::string(16, ' ') << '\r';

    m_window->processMessages();

    for (auto &[button, state] : m_mouseButtonPool)
      onMouseButton(button, state);
    m_mouseButtonPool.clear();

    for (auto &[x, y] : m_mouseMovePool)
      onMouseMove(x, y);
    m_mouseMovePool.clear();

    for (auto &[key, state] : m_keyPool)
      onKey(key, state);
    m_keyPool.clear();

    m_admin->cacheSystems();
    m_admin->updateSystems(dt, tickCount);

    if (!update(dt, tickCount))
      break;

    // Update the renderer system
    m_renderer->update(dt, tickCount);
    ++tickCount;
  }
  return true;
}

Game::Game(int argc, char **argv, char **env) : ApplicationBase{argc, argv, env} {
  m_audioManager = std::make_unique<AudioManager>();
  if (m_audioManager == nullptr)
    throw std::runtime_error{"Couldn't create AudioManager"};

  m_audioManager->startStream();

  m_window = std::make_unique<Windows::Window>();
  if (m_window == nullptr)
    throw std::runtime_error{"Couldn't create Windows::Window"};

  m_window->onMouseButton = [&](Input::MouseButton button, Input::State state) {
    m_mouseButtonPool.emplace_back(button, state);
  };

  m_window->onMouseMove = [&](float x, float y) {
    m_mouseMovePool.emplace_back(x, y);
  };

  m_window->onKey = [&](Input::Key key, Input::State state) {
    m_keyPool.emplace_back(key, state);
  };

  m_admin = std::make_unique<ECS::Admin>();
  if (m_admin == nullptr)
    throw std::runtime_error{"Couldn't create ECS::Admin"};

  m_renderer = std::make_unique<Vulkan::Renderer>(*(m_admin.get()), *(m_window.get()));
  if (m_renderer == nullptr)
    throw std::runtime_error{"Couldn't create Vulkan::Renderer"};
}

Game::~Game() {
  m_audioManager->stopStream();
  m_admin.reset();
}
