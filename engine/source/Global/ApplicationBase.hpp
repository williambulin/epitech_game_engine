#pragma once

#include "Extension/Language/Library.hpp"
#include "Log/Log.hpp"

#include <string>
#include <vector>
#include <map>

class ApplicationBase {
public:
  using Argument      = std::string;
  using Arguments     = std::vector<Argument>;
  using Environnement = std::map<Argument, Argument>;

private:
  Argument      m_applicationName{};
  Arguments     m_arguments{};
  Environnement m_environnement{};

public:
  DLLATTRIB explicit ApplicationBase(int argc = 0, char **argv = nullptr, char **env = nullptr);
  ApplicationBase(const ApplicationBase &) = delete;
  DLLATTRIB virtual ~ApplicationBase()     = default;

  auto operator=(const ApplicationBase &) -> ApplicationBase & = delete;

  [[nodiscard]] DLLATTRIB auto getApplicationName() const -> const Argument &;
  [[nodiscard]] DLLATTRIB auto getArguments() const -> const Arguments &;
  [[nodiscard]] DLLATTRIB auto getEnvironnement() const -> const Environnement &;

  [[nodiscard]] DLLATTRIB int          run();
  [[nodiscard]] DLLATTRIB virtual bool start() = 0;
};
