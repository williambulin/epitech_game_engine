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

class FromAssimp {
public:
  static inline Quaternion Quaternion(const aiQuaterniont<float> &);

  static inline std::string String(const aiString &);

  template <class T>
  static inline Vector<T, 2> Vector2(const aiVector2t<T> &);

  template <class T>
  static inline Vector<T, 2> Vector2(const aiVector3t<T> &);

  template <class T>
  static inline Vector<T, 3> Vector3(const aiVector3t<T> &);
};