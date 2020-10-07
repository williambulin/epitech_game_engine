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
  void                     operator=(const Vector2<T> &v);
  [[nodiscard]] bool       operator!=(const Vector2<T> &v);
  [[nodiscard]] bool       operator==(const Vector2<T> &v);
  [[nodiscard]] bool       operator>(const Vector2<T> &v);
  [[nodiscard]] bool       operator<(const Vector2<T> &v);
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool       operator>=(const Vector2<T> &v);
  [[nodiscard]] bool       operator<=(const Vector2<T> &v);
  [[nodiscard]] Vector2<T> operator+(const Vector2<T> &v);
  [[nodiscard]] Vector2<T> operator-(const Vector2<T> & v);
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
inline void Vector2<T>::operator=(const Vector2<T> &v) {
  x = v.x;
  y = V.y;
}

template <class T>
inline bool Vector2<T>::operator!=(const Vector2<T> &v) {
  return !(*this == v);
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
Vector2<T> Vector2<T>::operator+(const Vector2<T> & v) {
  return Vector2<T>{x + v.x, y + v.y};
}

template <class T>
Vector2<T> Vector2<T>::operator-(const Vector2<T> & v) {
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
inline Vector2<T> Vector2<T>::operator+=(const Vector2<T> & v) {
  x += v.x;
  y += v.y;
  return *this;
}

template <class T>
inline Vector2<T> Vector2<T>::operator-=(const Vector2<T> & v) {
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
  void               operator=(const Vector3<T> &v);
  [[nodiscard]] bool operator!=(const Vector3<T> &v);
  [[nodiscard]] bool operator==(const Vector3<T> &v);
  [[nodiscard]] bool operator>(const Vector3<T> &v);
  [[nodiscard]] bool operator<(const Vector3<T> &v);
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool       operator>=(const Vector3<T> &v);
  [[nodiscard]] bool       operator<=(const Vector3<T> &v);
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
inline void Vector3<T>::operator=(const Vector3<T> &v) {
  x = v.x;
  y = v.y;
  z = v.z;
}

template <class T>
inline bool Vector3<T>::operator!=(const Vector3<T> &v) {
  return !(*this == v);
}

template <class T>
inline bool Vector3<T>::operator==(const Vector3<T> &v) {
  return x == v.x && y == v.y && z == v.z;
}

template <class T>
inline bool Vector3<T>::operator>(const Vector3<T> &v) {
  return x > v.x && y > v.y && z > v.z;
}

template <class T>
inline bool Vector3<T>::operator<(const Vector3<T> &v) {
  return x < v.x && y < v.y && z < v.z;
}

template <class T>
inline bool Vector3<T>::operator>=(const Vector3<T> &v) {
  return x >= v.x && y >= v.y && z >= v.z;
}

template <class T>
inline bool Vector3<T>::operator<=(const Vector3<T> &v) {
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
  return Vector3<T>{x * v.x, y * v.y, z* v.z};
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
  explicit Vector(std::array<T, size> array);
  explicit Vector(std::vector<T> array);

  ~Vector()                 = default;
  void               operator=(const Vector<T, size> &v);
  [[nodiscard]] bool operator!=(const Vector<T, size> &v);
  [[nodiscard]] bool operator==(const Vector<T, size> &v);
  [[nodiscard]] bool operator>(const Vector<T, size> &v);
  [[nodiscard]] bool operator<(const Vector<T, size> &v);
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool       operator>=(const Vector<T, size> &v);
  [[nodiscard]] bool                          operator<=(const Vector<T, size> &v);
  [[nodiscard]] T& operator[](uint32_t);
  [[nodiscard]] T &             operator()(uint32_t);
  [[nodiscard]] Vector<T, size> operator+(const Vector<T, size> &v);
  [[nodiscard]] Vector<T, size> operator-(const Vector<T, size> &v);
  [[nodiscard]] Vector<T, size> operator*(const Vector<T, size> &v);
  [[nodiscard]] Vector<T, size> operator/(const Vector<T, size> &v);
  [[nodiscard]] Vector<T, size> operator^(const Vector<T, size> &v);
  Vector<T, size>               operator+=(const Vector<T, size> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector<T, size>               operator-=(const Vector<T, size> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector<T, size>               operator*=(const Vector<T, size> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Vector<T, size>               operator/=(const Vector<T, size> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
};

template <class T, uint32_t size>
inline Vector<T, size>::Vector() : m_array{} {}

template <class T, uint32_t size>
inline Vector<T, size>::Vector(std::array<T, size> array) : m_array{array} {}

template <class T, uint32_t size>
inline Vector<T, size>::Vector(std::vector<T> array) : m_array{} {
  std::copy_n(array.begin(), size, m_array.begin());
}

template <class T, uint32_t size>
inline void Vector<T, size>::operator=(const Vector<T, size> &v) {
  std::copy_n(v.m_array.begin(), size, m_array.begin());
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator!=(const Vector<T, size> &v) {
  return m_array != v.m_array; // will probably replace by SIMD
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator==(const Vector<T, size> &v) {
  return m_array == v.m_array;  // will probably replace by SIMD
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator>(const Vector<T, size> &v) {
  for (auto p : m_array, q : v.m_array) {
    if (p <= q)
      return false;
  }
    return true;
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator<(const Vector<T, size> &v) {
  for (auto p : m_array, q : v.m_array) {
    if (p >= q)
      return false;
  }
  return true;
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator>=(const Vector<T, size> &v) {
  for (auto p : m_array, q : v.m_array) {
    if (p < q)
      return false;
  }
  return true;
}

template <class T, uint32_t size>
inline bool Vector<T, size>::operator<=(const Vector<T, size> &v) {
  for (auto p : m_array, q : v.m_array) {
    if (p > q)
      return false;
  }
  return true;
}

template <class T, uint32_t size>
inline T &Vector<T, size>::operator[](uint32_t i) {
  return m_array[i];
}

template <class T, uint32_t size>
inline T &Vector<T, size>::operator()(uint32_t) {
  return m_array[i];
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator+(const Vector<T, size> &v) {
  auto ret = Vector<T, size>();
  for (auto p : m_array, q : v.m_array, r : ret)
    r = p + q;
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator-(const Vector<T, size> &v) {
  auto ret = Vector<T, size>();
  for (auto p : m_array, q : v.m_array, r : ret)
    r = p - q;
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator*(const Vector<T, size> &v) {
  auto ret = Vector<T, size>();
  for (auto p : m_array, q : v.m_array, r : ret)
    r = p * q;
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator/(const Vector<T, size> &v) {
  auto ret = Vector<T, size>();
  for (auto p : m_array, q : v.m_array, r : ret) {
    if (p == 0.0f)
      throw std::runtime_error{"try to divide by 0"};
    r = p / q;
  }
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator^(const Vector<T, size> &v) {
  auto ret = Vector<T, size>();
  for (auto p : m_array, q : v.m_array, r : ret)
    r = p ^ q;
  return ret;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator+=(const Vector<T, size> &v) {
  for (auto p : m_array, q : v.m_array)
    p += q;
  return *this;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator-=(const Vector<T, size> &v) {
  for (auto p : m_array, q : v.m_array)
    p -= q;
  return *this;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator*=(const Vector<T, size> &v) {
  for (auto p : m_array, q : v.m_array)
    p *= q;
  return *this;
}

template <class T, uint32_t size>
inline Vector<T, size> Vector<T, size>::operator/=(const Vector<T, size> &v) {
  for (auto p : m_array, q : v.m_array) {
    if (p == 0.0f)
      throw std::runtime_error{"try to divide by 0"};
    p += q;
  }
  return *this;
}
