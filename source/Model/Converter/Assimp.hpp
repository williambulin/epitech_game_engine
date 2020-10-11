#pragma once

#include <assimp/vector2.h>
#include <assimp/vector3.h>

#include "../../Maths/Vectors.hpp"

class FromAssimp {
public:
  template <class T>
  static Vector<T, 2> Vector2(const aiVector2t<T> &);

  template <class T>
  static Vector<T, 2> Vector2(const aiVector3t<T> &);

  template <class T>
  static Vector<T, 3> Vector3(const aiVector3t<T> &);
};