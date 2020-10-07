#pragma once

template <class T>
class Vector2 final {
public:
  T x;
  T y;

public:
  explicit Vector2();
  explicit Vector2(T a, T b);

  ~Vector2() = default;
  void                     operator=(Vector2<T> const &v);
  [[nodiscard]] bool       operator!=(Vector2<T> const &v);
  [[nodiscard]] bool       operator==(Vector2<T> const &v);
  [[nodiscard]] bool       operator>(Vector2<T> const &v);
  [[nodiscard]] bool       operator<(Vector2<T> const &v);
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool       operator>=(Vector2<T> const &v);
  [[nodiscard]] bool       operator<=(Vector2<T> const &v);
  [[nodiscard]] Vector2<T> operator+(Vector2<T> const &v);
  [[nodiscard]] Vector2<T> operator-(Vector2<T> const & v);
  [[nodiscard]] Vector2<T> operator*(Vector2<T> const &v);
  [[nodiscard]] Vector2<T> operator/(Vector2<T> const &v);
  [[nodiscard]] Vector2<T> operator^(Vector2<T> const &v);
  Vector2<T>               operator+=(Vector2<T> const &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector2<T>               operator-=(Vector2<T> const &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector2<T>               operator*=(Vector2<T> const &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector2<T>               operator/=(Vector2<T> const &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
};

template <class T>
inline Vector2<T>::Vector2() : x{0},
                               y{0} {}

template <class T>
Vector2<T>::Vector2(T a, T b) : x{a},
                                y{b} {}

template <class T>
inline void Vector2<T>::operator=(Vector2<T> const &v) {
  x = v.x;
  y = V.y;
}

template <class T>
inline bool Vector2<T>::operator!=(Vector2<T> const &v) {
  return !(*this == v);
}

template <class T>
inline bool Vector2<T>::operator==(Vector2<T> const &v) {
  return x == v.x && y == v.y;
}

template <class T>
inline bool Vector2<T>::operator>(Vector2<T> const &v) {
  return x > v.x && y > v.y;
}

template <class T>
inline bool Vector2<T>::operator<(Vector2<T> const &v) {
  return x < v.x && y < v.y;
}

template <class T>
inline bool Vector2<T>::operator>=(Vector2<T> const &v) {
  return x >= v.x && y >= v.y;
}

template <class T>
inline bool Vector2<T>::operator<=(Vector2<T> const &v) {
  return x <= v.x && y <= v.y;
}

template <class T>
Vector2<T> Vector2<T>::operator+(Vector2<T> const & v) {
  return Vector2<T>{x + v.x, y + v.y};
}

template <class T>
Vector2<T> Vector2<T>::operator-(Vector2<T> const & v) {
    return Vector2<T>{x - v.x, y - v.y};
}

template <class T>
inline Vector2<T> Vector2<T>::operator*(Vector2<T> const &v) {
  return Vector2<T>{x * v.x, y * v.y};
}

template <class T>
inline Vector2<T> Vector2<T>::operator/(Vector2<T> const &v) {
  if (!x || !y)
    throw std::runtime_error("try to divise by 0");
  return Vector2<T>{x / v.x, y / v.y};
}

template <class T>
inline Vector2<T> Vector2<T>::operator^(Vector2<T> const &v) {
  return Vector2<T>{x ^ v.x, y ^ v.y};
}

template <class T>
inline Vector2<T> Vector2<T>::operator+=(Vector2<T> const & v) {
  x += v.x;
  y += v.y;
  return *this;
}

template <class T>
inline Vector2<T> Vector2<T>::operator-=(Vector2<T> const & v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

template <class T>
inline Vector2<T> Vector2<T>::operator*=(Vector2<T> const &v) {
  x *= v.x;
  y *= v.y;
  return *this;
}

template <class T>
inline Vector2<T> Vector2<T>::operator/=(Vector2<T> const &v) {
  x /= v.x;
  y /= v.y;
  return *this;
}

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;

template <class T>
class Vector3 final {
public:
  T x;
  T y;
  T z;

public:
  explicit Vector3();
  explicit Vector3(T a, T b, T c);

  ~Vector3()                 = default;
  void               operator=(Vector3<T> const &v);
  [[nodiscard]] bool operator!=(Vector3<T> const &v);
  [[nodiscard]] bool operator==(Vector3<T> const &v);
  [[nodiscard]] bool operator>(Vector3<T> const &v);
  [[nodiscard]] bool operator<(Vector3<T> const &v);
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool       operator>=(Vector3<T> const &v);
  [[nodiscard]] bool       operator<=(Vector3<T> const &v);
  [[nodiscard]] Vector3<T> operator+(Vector3<T> const &v);
  [[nodiscard]] Vector3<T> operator-(Vector3<T> const &v);
  [[nodiscard]] Vector3<T> operator*(Vector3<T> const &v);
  [[nodiscard]] Vector3<T> operator/(Vector3<T> const &v);
  [[nodiscard]] Vector3<T> operator^(Vector3<T> const &v);
  Vector3<T>               operator+=(Vector3<T> const &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector3<T>               operator-=(Vector3<T> const &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector3<T>               operator*=(Vector3<T> const &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector3<T>               operator/=(Vector3<T> const &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
};

template <class T>
inline Vector3<T>::Vector3() : x{0},
                               y{0},
                               z{0} {}

template <class T>
Vector3<T>::Vector3(T a, T b, T c) : x{a},
                                     y{b},
                                     z{c} {}

template <class T>
inline void Vector3<T>::operator=(Vector3<T> const &v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

template <class T>
inline bool Vector3<T>::operator!=(Vector3<T> const &v) {
  return !(*this == v);
}

template <class T>
inline bool Vector3<T>::operator==(Vector3<T> const &v) {
  return x == v.x && y == v.y && z == v.z;
}

template <class T>
inline bool Vector3<T>::operator>(Vector3<T> const &v) {
  return x > v.x && y > v.y && z > v.z;
}

template <class T>
inline bool Vector3<T>::operator<(Vector3<T> const &v) {
  return x < v.x && y < v.y && z < v.z;
}

template <class T>
inline bool Vector3<T>::operator>=(Vector3<T> const &v) {
  return x >= v.x && y >= v.y && z >= v.z;
}

template <class T>
inline bool Vector3<T>::operator<=(Vector3<T> const &v) {
  return x <= v.x && y <= v.y && z <= v.z;
}

template <class T>
Vector3<T> Vector3<T>::operator+(Vector3<T> const &v) {
  return Vector3<T>{x + v.x, y + v.y, z + v.z};
}

template <class T>
Vector3<T> Vector3<T>::operator-(Vector3<T> const &v) {
  return Vector3<T>{x - v.x, y - v.y, z - v.z};
}

template <class T>
inline Vector3<T> Vector3<T>::operator*(Vector3<T> const &v) {
  return Vector3<T>{x * v.x, y * v.y, z* v.z};
}

template <class T>
inline Vector3<T> Vector3<T>::operator/(Vector3<T> const &v) {
  if (!x || !y || !z)
    throw std::runtime_error{"try to divise by 0"};
  return Vector3<T>{x / v.x, y / v.y, z / v.z};
}

template <class T>
inline Vector3<T> Vector3<T>::operator^(Vector3<T> const &v) {
  return Vector3<T>{x ^ v.x, y ^ v.y, z ^ v.z};
}

template <class T>
inline Vector3<T> Vector3<T>::operator+=(Vector3<T> const &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

template <class T>
inline Vector3<T> Vector3<T>::operator-=(Vector3<T> const &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

template <class T>
inline Vector3<T> Vector3<T>::operator*=(Vector3<T> const &v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}

template <class T>
inline Vector3<T> Vector3<T>::operator/=(Vector3<T> const &v) {
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;
