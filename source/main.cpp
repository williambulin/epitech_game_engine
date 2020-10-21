#include "Core.hpp"
#include "Physics/Collision.hpp"
#include "Physics/Transform.hpp"
#include <iostream>

int main(int argc, char **argv, char **env) {
  Core app{argc, argv, env};
  Vector3f firstMin(1.0f, 1.0f, 1.0f);
  Vector3f firstMax(5.0f, 5.0f, 5.0f);
  std::cout << firstMin.x << " | " << firstMin.y << " | " << firstMin.z << std::endl;
  std::cout << firstMax.x << " | " << firstMax.y << " | " << firstMax.z << std::endl;
  AABB first(firstMin, firstMax);
  Vector3f secondMin{3.5f, 2.0f, 0.0f};
  Vector3f secondMax{7.0f, 5.5f, 3.5f};
  std::cout << secondMin.x << " | " << secondMin.y << " | " << secondMin.z << std::endl;
  std::cout << secondMax.x << " | " << secondMax.y << " | " << secondMax.z << std::endl;

  AABB second(secondMin, secondMax);
  CollisionInfo info;
  bool return_value = Collision::collide(first, Transform(), second, Transform(), info);
  return app.run();
}
