#include "Assimp.hpp"

#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/types.h>
#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <string>

#include "../../Maths/Matrix.hpp"
#include "../../Maths/Quaternion.hpp"
#include "../../Maths/Vectors.hpp"

Quaternion FromAssimp::Quaternion(const aiQuaterniont<float> &q) {
  return Quaternion(q.x, q.y, q.z, q.w);
}

std::string FromAssimp::String(const aiString& s) {
  return std::string(s.C_Str());
}

template<class T>
Vector<T, 2> FromAssimp::Vector2(const aiVector2t<T> &v) {
  return Vector<T, 2>(v.x, v.y);
}

template <class T>
Vector<T, 2> FromAssimp::Vector2(const aiVector3t<T> &v) {
  return Vector<T, 2>(v.x, v.y);
}

template<class T>
Vector<T, 3> FromAssimp::Vector3(const aiVector3t<T> &v) {
  return Vector<T, 3>(v.x, v.y, v.z);
}