#pragma once

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

class FromAssimp final {
public:
  template <class T>
  static Matrix4<T> mat4(const aiMatrix4x4t<T> &);

  static Quaternion quat(const aiQuaterniont<float> &);

  static std::string str(const aiString &);

  template <class T>
  static Vector2<T> vec2(const aiVector2t<T> &);

  template <class T>
  static Vector2<T> vec2(const aiVector3t<T> &);

  template <class T>
  static Vector3<T> vec3(const aiVector3t<T> &);
};