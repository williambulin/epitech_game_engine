#include "Core.hpp"
#include "Model/OBJ.hpp"

int main(int argc, char **argv, char **env) {
  OBJ  test{"../../../resource/disk.obj"};
  Core app{argc, argv, env};
  return app.run();
}
