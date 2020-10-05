#include <sstream>
#include <iostream>

#include "ApplicationBase.hpp"
#include "Extension/Language/Keywords.hpp"
#include "Extension/Language/Platform.hpp"

#if defined(OS_WINDOWS)
#include <Windows.h>
#endif

ApplicationBase::ApplicationBase(int argc, char **argv, char **env) {
  if (argc >= 1 && argv != nullptr) {
    m_applicationName = argv[0];

    for (int i{1}; i < argc; ++i)
      m_arguments.push_back(argv[i]);
  }

  if (env != nullptr) {
    for (std::size_t i{0}; env[i] != nullptr; ++i) {
      std::stringstream ss{};
      ss << env[i];

      std::string key{};
      std::string value{};
      std::getline(ss, key, '=');
      std::getline(ss, value);
      m_environnement[key] = value;
    }
  }
}

int ApplicationBase::ApplicationBase::run() {
  try {
    return start() ? 0 : 1;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';

#if defined(OS_WINDOWS)
    MessageBoxA(nullptr, e.what(), "An unhandled exception has occured", MB_ICONERROR);
#endif
  }
  return 1;
}

auto ApplicationBase::getApplicationName() const -> const Argument & {
  return m_applicationName;
}

auto ApplicationBase::getArguments() const -> const Arguments & {
  return m_arguments;
}

auto ApplicationBase::getEnvironnement() const -> const Environnement & {
  return m_environnement;
}
