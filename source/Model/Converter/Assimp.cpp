#include "Assimp.hpp"

#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/types.h>
#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <string>
#include <vector>

#include "../../Maths/Matrix.hpp"
#include "../../Maths/Quaternion.hpp"
#include "../../Maths/Vectors.hpp"

template <class T>
Matrix4<T> FromAssimp::mat4(const aiMatrix4x4t<T> &m) {
  return Matrix4<T>(std::vector<std::vector<T>>{{m.a1, m.b1, m.c1, m.d1}, {m.a2, m.b2, m.c2, m.d2}, {m.a3, m.b3, m.c3, m.d3}, {m.a4, m.b4, m.c4, m.d4}});
}

Quaternion FromAssimp::quat(const aiQuaterniont<float> &q) {
  return Quaternion(q.x, q.y, q.z, q.w);
}

std::string FromAssimp::str(const aiString &s) {
  return std::string(s.C_Str());
}

template <class T>
Vector2<T> FromAssimp::vec2(const aiVector2t<T> &v) {
  return Vector2<T>(v.x, v.y);
}

template <class T>
Vector2<T> FromAssimp::vec2(const aiVector3t<T> &v) {
  return Vector2<T>(v.x, v.y);
}

template <class T>
Vector3<T> FromAssimp::vec3(const aiVector3t<T> &v) {
  return Vector3<T>(v.x, v.y, v.z);
}