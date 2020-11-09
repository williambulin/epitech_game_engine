#include "Core.hpp"

int main(int argc, char **argv, char **env) {
  Core app{argc, argv, env};
  return app.run();
}
