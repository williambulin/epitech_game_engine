#pragma once

#include <stdexcept>
#include <vector>
template <class T>
class Vector2 final {
public:
  T x;
  T y;

public:
  explicit Vector2();
  explicit Vector2(T a, T b);

  ~Vector2()                 = default;
  Vector2<T> &       operator=(const Vector2<T> &v);
  [[nodiscard]] bool operator!=(const Vector2<T> &v);
  [[nodiscard]] bool operator==(const Vector2<T> &v);
  [[nodiscard]] bool operator>(const Vector2<T> &v);
  [[nodiscard]] bool operator<(const Vector2<T> &v);
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool       operator>=(const Vector2<T> &v);
  [[nodiscard]] bool       operator<=(const Vector2<T> &v);
  [[nodiscard]] Vector2<T> operator+(const Vector2<T> &v);
  [[nodiscard]] Vector2<T> operator-(const Vector2<T> &v);
  [[nodiscard]] Vector2<T> operator*(const Vector2<T> &v);
  [[nodiscard]] Vector2<T> operator/(const Vector2<T> &v);
  [[nodiscard]] Vector2<T> operator^(const Vector2<T> &v);
  Vector2<T>               operator+=(const Vector2<T> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector2<T>               operator-=(const Vector2<T> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector2<T>               operator*=(const Vector2<T> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector2<T>               operator/=(const Vector2<T> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
};

template <class T>
inline Vector2<T>::Vector2() : x{0},
                               y{0} {}

template <class T>
Vector2<T>::Vector2(T a, T b) : x{a},
                                y{b} {}

template <class T>
inline Vector2<T> &Vector2<T>::operator=(const Vector2<T> &v) {
  x = v.x;
  y = v.y;
  return *this;
}

template <class T>
inline bool Vector2<T>::operator!=(const Vector2<T> &v) {
  return x != v.x && y != v.y;
}

template <class T>
inline bool Vector2<T>::operator==(const Vector2<T> &v) {
  return x == v.x && y == v.y;
}

template <class T>
inline bool Vector2<T>::operator>(const Vector2<T> &v) {
  return x > v.x && y > v.y;
}

template <class T>
inline bool Vector2<T>::operator<(const Vector2<T> &v) {
  return x < v.x && y < v.y;
}

template <class T>
inline bool Vector2<T>::operator>=(const Vector2<T> &v) {
  return x >= v.x && y >= v.y;
}

template <class T>
inline bool Vector2<T>::operator<=(const Vector2<T> &v) {
  return x <= v.x && y <= v.y;
}

template <class T>
Vector2<T> Vector2<T>::operator+(const Vector2<T> &v) {
  return Vector2<T>{x + v.x, y + v.y};
}

template <class T>
Vector2<T> Vector2<T>::operator-(const Vector2<T> &v) {
  return Vector2<T>{x - v.x, y - v.y};
}

template <class T>
inline Vector2<T> Vector2<T>::operator*(const Vector2<T> &v) {
  return Vector2<T>{x * v.x, y * v.y};
}

template <class T>
inline Vector2<T> Vector2<T>::operator/(const Vector2<T> &v) {
  if (x == 0.0f || y == 0.0f)
    throw std::runtime_error{"try to divide by 0"};
  return Vector2<T>{x / v.x, y / v.y};
}

template <class T>
inline Vector2<T> Vector2<T>::operator^(const Vector2<T> &v) {
  return Vector2<T>{x ^ v.x, y ^ v.y};
}

template <class T>
inline Vector2<T> Vector2<T>::operator+=(const Vector2<T> &v) {
  x += v.x;
  y += v.y;
  return *this;
}

template <class T>
inline Vector2<T> Vector2<T>::operator-=(const Vector2<T> &v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

template <class T>
inline Vector2<T> Vector2<T>::operator*=(const Vector2<T> &v) {
  x *= v.x;
  y *= v.y;
  return *this;
}

template <class T>
inline Vector2<T> Vector2<T>::operator/=(const Vector2<T> &v) {
  if (x == 0.0f || y == 0.0f)
    throw std::runtime_error{"try to divide by 0"};
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
  Vector3<T> &       operator=(const Vector3<T> &v);
  [[nodiscard]] bool operator!=(const Vector3<T> &v) const;
  [[nodiscard]] bool operator==(const Vector3<T> &v) const;
  [[nodiscard]] bool operator>(const Vector3<T> &v) const;
  [[nodiscard]] bool operator<(const Vector3<T> &v) const;
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool       operator>=(const Vector3<T> &v) const;
  [[nodiscard]] bool       operator<=(const Vector3<T> &v) const;
  [[nodiscard]] Vector3<T> operator+(const Vector3<T> &v);
  [[nodiscard]] Vector3<T> operator-(const Vector3<T> &v);
  [[nodiscard]] Vector3<T> operator*(const Vector3<T> &v);
  [[nodiscard]] Vector3<T> operator/(const Vector3<T> &v);
  [[nodiscard]] Vector3<T> operator^(const Vector3<T> &v);
  Vector3<T>               operator+=(const Vector3<T> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector3<T>               operator-=(const Vector3<T> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector3<T>               operator*=(const Vector3<T> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector3<T>               operator/=(const Vector3<T> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
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
inline Vector3<T> &Vector3<T>::operator=(const Vector3<T> &v) {
  x = v.x;
  y = v.y;
  z = v.z;
  return *this;
}

template <class T>
inline bool Vector3<T>::operator!=(const Vector3<T> &v) const {
  return x != v.x && y != v.y && z != v.z;
}

template <class T>
inline bool Vector3<T>::operator==(const Vector3<T> &v) const {
  return x == v.x && y == v.y && z == v.z;
}

template <class T>
inline bool Vector3<T>::operator>(const Vector3<T> &v) const {
  return x > v.x && y > v.y && z > v.z;
}

template <class T>
inline bool Vector3<T>::operator<(const Vector3<T> &v) const {
  return x < v.x && y < v.y && z < v.z;
}

template <class T>
inline bool Vector3<T>::operator>=(const Vector3<T> &v) const {
  return x >= v.x && y >= v.y && z >= v.z;
}

template <class T>
inline bool Vector3<T>::operator<=(const Vector3<T> &v) const {
  return x <= v.x && y <= v.y && z <= v.z;
}

template <class T>
Vector3<T> Vector3<T>::operator+(const Vector3<T> &v) {
  return Vector3<T>{x + v.x, y + v.y, z + v.z};
}

template <class T>
Vector3<T> Vector3<T>::operator-(const Vector3<T> &v) {
  return Vector3<T>{x - v.x, y - v.y, z - v.z};
}

template <class T>
inline Vector3<T> Vector3<T>::operator*(const Vector3<T> &v) {
  return Vector3<T>{x * v.x, y * v.y, z * v.z};
}

template <class T>
inline Vector3<T> Vector3<T>::operator/(const Vector3<T> &v) {
  if (x == 0.0f || y == 0.0f || z == 0.0f)
    throw std::runtime_error{"try to divide by 0"};
  return Vector3<T>{x / v.x, y / v.y, z / v.z};
}

template <class T>
inline Vector3<T> Vector3<T>::operator^(const Vector3<T> &v) {
  return Vector3<T>{x ^ v.x, y ^ v.y, z ^ v.z};
}

template <class T>
inline Vector3<T> Vector3<T>::operator+=(const Vector3<T> &v) {
  x += v.x;
  y += v.y;
  z += v.z;
  return *this;
}

template <class T>
inline Vector3<T> Vector3<T>::operator-=(const Vector3<T> &v) {
  x -= v.x;
  y -= v.y;
  z -= v.z;
  return *this;
}

template <class T>
inline Vector3<T> Vector3<T>::operator*=(const Vector3<T> &v) {
  x *= v.x;
  y *= v.y;
  z *= v.z;
  return *this;
}

template <class T>
inline Vector3<T> Vector3<T>::operator/=(const Vector3<T> &v) {
  if (x == 0.0f || y == 0.0f || z == 0.0f)
    throw std::runtime_error{"try to divide by 0"};
  x /= v.x;
  y /= v.y;
  z /= v.z;
  return *this;
}

using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;

template <class T, uint32_t size>
class Vector final {
private:
  std::array<T, size> m_array;

public:
  explicit Vector();
  explicit Vector(const std::array<T, size> &array);
  explicit Vector(const std::vector<T> &array);

  ~Vector()                  = default;
  Vector<T, size> &  operator=(const Vector<T, size> &v);
  [[nodiscard]] bool operator!=(const Vector<T, size> &v) const;
  [[nodiscard]] bool operator==(const Vector<T, size> &v) const;
  [[nodiscard]] bool operator==(const float &v) const;
  [[nodiscard]] bool operator>(const Vector<T, size> &v) const;
  [[nodiscard]] bool operator<(const Vector<T, size> &v) const;
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool            operator>=(const Vector<T, size> &v) const;
  [[nodiscard]] bool            operator<=(const Vector<T, size> &v) const;
  [[nodiscard]] T &             operator[](uint32_t);
  [[nodiscard]] T               operator[](uint32_t) const;
  [[nodiscard]] T &             operator()(uint32_t);
  [[nodiscard]] Vector<T, size> operator+(const Vector<T, size> &v);
  [[nodiscard]] Vector<T, size> operator-(const Vector<T, size> &v);
  [[nodiscard]] Vector<T, size> operator*(const Vector<T, size> &v);
  [[nodiscard]] Vector<T, size> operator*(const int &v);
  [[nodiscard]] Vector<T, size> operator*(const float &v);
  [[nodiscard]] Vector<T, size> operator/(const Vector<T, size> &v);
  [[nodiscard]] Vector<T, size> operator^(const Vector<T, size> &v);
  Vector<T, size> &             operator+=(const Vector<T, size> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector<T, size> &             operator-=(const Vector<T, size> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector<T, size> &             operator*=(const int &v);
  Vector<T, size> &             operator*=(const float &v);
  Vector<T, size> &             operator*=(const Vector<T, size> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector<T, size> &             operator/=(const Vector<T, size> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector<T, size> &             lerp(const Vector<T, size> &_end, float percent);
};

template <class T, uint32_t size>
Vector<T, size> &Vector<T, size>::operator*=(const int &v) {
  T *this_data = m_array.data();
  for (auto &data : m_array)
    data *= v;
  return *this;
}

template <class T, uint32_t size>
Vector<T, size> &Vector<T, size>::operator*=(const float &v) {
  T *this_data = m_array.data();
  for (auto &data : m_array)
    data *= v;
  return *this;
}

template <class T, uint32_t size>
inline Vector<T, size>::Vector() : m_array{} {}

template <class T, uint32_t size>
inline Vector<T, size>::Vector(const std::array<T, size> &array) : m_array{array} {}

template <class T, uint32_t size>
inline Vector<T, size>::Vector(const std::vector<T> &array) : m_array{} {
  std::copy_n(array.begin(), size, m_array.begin());
}

template <class T, uint32_t size>
inline Vector<T, size> &Vector<T, size>::operator=(const Vector<T, size> &v) {
  std::copy_n(v.m_array.begin(), size, m_array.begin());
  return *this;
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator!=(const Vector<T, size> &v) const {
  return m_array != v.m_array;  // will probably replace by SIMD
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator==(const Vector<T, size> &v) const {
  return m_array == v.m_array;  // will probably replace by SIMD
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator>(const Vector<T, size> &v) const {
  T *this_data  = m_array.data();
  T *other_data = v.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data) {
    if (*this_data <= *other_data)
      return false;
  }
  return true;
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator<(const Vector<T, size> &v) const {
  T *this_data  = m_array.data();
  T *other_data = v.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data) {
    if (*this_data >= *other_data)
      return false;
  }
  return true;
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator>=(const Vector<T, size> &v) const {
  T *this_data  = m_array.data();
  T *other_data = v.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data) {
    if (*this_data < *other_data)
      return false;
  }
  return true;
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator<=(const Vector<T, size> &v) const {
  T *this_data  = m_array.data();
  T *other_data = v.m_array;
  for (int i = 0; i < size; ++i, ++this_data, ++other_data) {
    if (*this_data > *other_data)
      return false;
  }
  return true;
}

template <class T, uint32_t size>
inline T &Vector<T, size>::operator[](uint32_t i) {
  return m_array[i];
}

template <class T, uint32_t size>
inline T &Vector<T, size>::operator()(uint32_t i) {
  return m_array[i];
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator+(const Vector<T, size> &v) {
  Vector<T, size> ret{m_array};
  const T *       other_data = v.m_array.data();
  T *             this_data  = ret.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data)
    (*this_data) += (*other_data);
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator-(const Vector<T, size> &v) {
  Vector<T, size> ret{m_array};
  const T *       other_data = v.m_array.data();
  T *             this_data  = ret.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data)
    *this_data -= *other_data;
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator*(const Vector<T, size> &v) {
  Vector<T, size> ret{m_array};
  const T *       other_data = v.m_array.data();
  T *             this_data  = ret.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data)
    *this_data *= *other_data;
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator/(const Vector<T, size> &v) {
  Vector<T, size> ret{m_array};
  const T *       other_data = v.m_array.data();
  T *             this_data  = ret.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data) {
    if ((*other_data) == 0.0f)
      throw std::runtime_error{"try to divide by 0"};
    *this_data /= *other_data;
  }
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator^(const Vector<T, size> &v) {
  Vector<T, size> ret{};
  T *             this_data  = m_array.data();
  T *             other_data = v.m_array.data();
  T *             ret_data   = ret.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data, ++ret_data)
    *ret_data = *this_data ^ *other_data;
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> &Vector<T, size>::operator+=(const Vector<T, size> &v) {
  T *      this_data  = m_array.data();
  const T *other_data = v.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data)
    *this_data += *other_data;
  return *this;
}

template <class T, uint32_t size>
inline Vector<T, size> &Vector<T, size>::operator-=(const Vector<T, size> &v) {
  T *      this_data  = m_array.data();
  const T *other_data = v.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data)
    *this_data -= *other_data;
  return *this;
}

template <class T, uint32_t size>
inline Vector<T, size> &Vector<T, size>::operator*=(const Vector<T, size> &v) {
  T *      this_data  = m_array.data();
  const T *other_data = v.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data)
    *this_data *= *other_data;
  return *this;
}

template <class T, uint32_t size>
inline Vector<T, size> &Vector<T, size>::operator/=(const Vector<T, size> &v) {
  T *      this_data  = m_array.data();
  const T *other_data = v.m_array.data();
  for (int i = 0; i < size; ++i, ++this_data, ++other_data) {
    if (*other_data == 0.0f)
      throw std::runtime_error{"try to divide by 0"};
    *this_data /= *other_data;
  }
  return *this;
}
template <class T, uint32_t size>
bool Vector<T, size>::operator==(const float &v) const {
  return false;
}
template <class T, uint32_t size>
Vector<T, size> Vector<T, size>::operator*(const int &v) {
  Vector<T, size> ret{m_array};
  for (auto &data : ret.m_array)
    data *= v;
  return ret;
}
template <class T, uint32_t size>
Vector<T, size> Vector<T, size>::operator*(const float &v) {
  Vector<T, size> ret{m_array};
  for (auto &data : ret.m_array)
    data *= v;
  return ret;
}
template <class T, uint32_t size>
T Vector<T, size>::operator[](uint32_t i) const {
  return m_array[i];
}

template <class T, uint32_t size>
Vector<T, size> &Vector<T, size>::lerp(const Vector<T, size> &_end, float percent) {
  return *this + percent*(_end - *this);
}
