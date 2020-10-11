#include "Assimp.hpp"

#include <assimp/vector2.h>
#include <assimp/vector3.h>

#include "../../Maths/Vectors.hpp"

template<class T>
static Vector<T, 2> FromAssimp::Vector2(const aiVector2t<T> &v) {
  return Vector<T, 2>(v.x, v.y);
}

template <class T>
static Vector<T, 2> FromAssimp::Vector2(const aiVector3t<T> &v) {
  return Vector<T, 2>(v.x, v.y);
}

template<class T>
static Vector<T, 3> FromAssimp::Vector3(const aiVector3t<T> &v) {
  return Vector<T, 3>(v.x, v.y, v.z);
}