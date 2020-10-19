#pragma once

#include <array>
#include <vector>
#include "Vectors.hpp"

template <class T, std::size_t width, std::size_t height>
class Matrix {
private:
  Vector<Vector<T, height>, width> m_matrix;

public:
  explicit Matrix();
  explicit Matrix(std::array<std::array<T, height>, width> array);
  explicit Matrix(std::vector<std::vector<T>> array);
  explicit Matrix(Vector<Vector<T, height>, width> array);

  ~Matrix()                         = default;
  Matrix<T, width, height> &operator=(const Matrix<T, width, height> &v);
  [[nodiscard]] bool        operator!=(const Matrix<T, width, height> &v);
  [[nodiscard]] bool        operator==(const Matrix<T, width, height> &v);
  [[nodiscard]] bool        operator>(const Matrix<T, width, height> &v);
  [[nodiscard]] bool        operator<(const Matrix<T, width, height> &v);
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool                     operator>=(const Matrix<T, width, height> &v);
  [[nodiscard]] bool                     operator<=(const Matrix<T, width, height> &v);
  [[nodiscard]] Vector<T, height> &      operator[](std::size_t);
  [[nodiscard]] Vector<T, height>        operator[](std::size_t) const;
  [[nodiscard]] Vector<T, height> &      operator()(std::size_t);
  [[nodiscard]] Matrix<T, width, height> operator+(const Matrix<T, width, height> &v);
  [[nodiscard]] Matrix<T, width, height> operator-(const Matrix<T, width, height> &v);
  [[nodiscard]] Matrix<T, width, height> operator*(const Matrix<T, width, height> &v);
  [[nodiscard]] Matrix<T, width, height> operator*(const int &v);
  [[nodiscard]] Matrix<T, width, height> operator*(const float &v);
  [[nodiscard]] Matrix<T, height, width> transpose();
  [[nodiscard]] Matrix<T, width, height> operator^(const Matrix<T, width, height> &v);
  Matrix<T, width, height>               operator+=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, height>               operator-=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, height>               operator*=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
};

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height>::Matrix() : m_matrix{} {}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height>::Matrix(std::array<std::array<T, height>, width> array) : m_matrix{array} {}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height>::Matrix(std::vector<std::vector<T>> array) : m_matrix{} {
  for (int i = 0; i < width; ++i)
    m_matrix[i] = Vector<T, height>{array[i]};
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> &Matrix<T, width, height>::operator=(const Matrix<T, width, height> &v) {
  for (int i = 0; i < width; ++i) {
    std::copy_n((&(v.m_matrix[i])[0]), height, (&(m_matrix[i])[0]));
  }
  return *this;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator!=(const Matrix<T, width, height> &v) {
  return m_matrix != v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator==(const Matrix<T, width, height> &v) {
  return m_matrix == v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator>(const Matrix<T, width, height> &v) {
  return m_matrix > v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator<(const Matrix<T, width, height> &v) {
  return m_matrix < v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator>=(const Matrix<T, width, height> &v) {
  return m_matrix >= v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator<=(const Matrix<T, width, height> &v) {
  return m_matrix <= v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline Vector<T, height> &Matrix<T, width, height>::operator[](std::size_t i) {
  return m_matrix[i];
}

template <class T, std::size_t width, std::size_t height>
inline Vector<T, height> &Matrix<T, width, height>::operator()(std::size_t i) {
  return m_matrix[i];
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator+(const Matrix<T, width, height> &v) {
  return Matrix<T, width, height>{m_matrix + v.m_matrix};
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator-(const Matrix<T, width, height> &v) {
  return Matrix<T, width, height>{m_matrix - v.m_matrix};
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator*(const Matrix<T, width, height> &v) {
  if (width != height)
    throw std::runtime_error{"cannot multiply a n*m matrix by a n*m matrix only n*n matrices by n*n matrices"};
  Matrix<T, width, width> ret{};  // width = height
  for (size_t i = 0; i < width; ++i) {
    for (size_t j = 0; j < width; ++j) {
      ret[i][j] = 0;
      for (size_t k = 0; k < width; ++k)
        ret[i][j] += (m_matrix[i][k]) * (v[k][j]);
    }
  }
  return ret;
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator^(const Matrix<T, width, height> &v) {
  return Matrix<T, width, height>{m_matrix ^ v.m_matrix};
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator+=(const Matrix<T, width, height> &v) {
  m_matrix += v.m_matrix;
  return *this;
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator-=(const Matrix<T, width, height> &v) {
  m_matrix -= v.m_matrix;
  return *this;
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator*=(const Matrix<T, width, height> &v) {
  if (width != height)
    throw std::runtime_error{"cannot multiply a n*m matrix by a n*m matrix only n*n matrices by n*n matrices"};
  *this = *this * v;  // we need to do a copy of *this to use it in the mul
  return *this;
}

template <class T, std::size_t width, std::size_t height>
Matrix<T, width, height>::Matrix(Vector<Vector<T, height>, width> array) : m_matrix{array} {}

template <class T, std::size_t width, std::size_t height>
Matrix<T, width, height> Matrix<T, width, height>::operator*(const int &v) {
  return Matrix<T, width, height>{m_matrix * v};
}
template <class T, std::size_t width, std::size_t height>
Matrix<T, width, height> Matrix<T, width, height>::operator*(const float &v) {
  Vector<Vector<T, height>, width> tmp = m_matrix * v;
  return Matrix<T, width, height>{tmp};
}
template <class T, std::size_t width, std::size_t height>
Matrix<T, height, width> Matrix<T, width, height>::transpose() {
  Matrix<T, height, width> ret{};
  for (int i = 0; i < height; ++i)
    for (int j = 0; j < width; ++j)
      ret[i][j] = m_matrix[j][i];
  return ret;
}
template <class T, std::size_t width, std::size_t height>
Vector<T, height> Matrix<T, width, height>::operator[](std::size_t i) const {
  return m_matrix[i];
}

template <class T>
class Matrix4 : public Matrix<T, 4, 4> {
public:
  static Matrix4<T> lookAt(const Vector<T, 3> &eye, const Vector<T, 3> &center, const Vector<T, 3> &up);
};

template <class T>
Matrix4<T> Matrix4<T>::lookAt(const Vector<T, 3> &eye, const Vector<T, 3> &center, const Vector<T, 3> &up) {
  T X, y = up, z = eye - center, XDotEye, yDotEye, zDotEye;
  z.normalize();
  X = y.cross(z);
  y = z.cross(X);
  X.normalize();
  y.normalize();
  return Matrix4<T>({{{X[0], X[1], X[2], -X.dot(eye)},{y[0], y[1], y[2], -y.dot(eye)}, {z[0], z[1], z[2], -z.dot(eye)}, {0, 0, 0, 1.0f}}});
}
