#pragma once

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
  explicit ApplicationBase(int argc = 0, char **argv = nullptr, char **env = nullptr);
  ApplicationBase(const ApplicationBase &) = delete;
  virtual ~ApplicationBase()               = default;

  auto operator=(const ApplicationBase &) -> ApplicationBase & = delete;

  [[nodiscard]] auto getApplicationName() const -> const Argument &;
  [[nodiscard]] auto getArguments() const -> const Arguments &;
  [[nodiscard]] auto getEnvironnement() const -> const Environnement &;

  [[nodiscard]] int          run();
  [[nodiscard]] virtual bool start() = 0;
};
