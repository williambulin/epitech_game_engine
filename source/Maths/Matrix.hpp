#pragma once

#include <array>
#include <vector>
#include "Vectors.hpp"

template <class T, std::size_t width, std::size_t height>
class Matrix {
protected:
  Vector<Vector<T, height>, width> m_matrix;

public:
  explicit Matrix();
  explicit Matrix(const std::array<std::array<T, height>, width> &array);
  explicit Matrix(const std::vector<std::vector<T>> &array);
  explicit Matrix(Vector<Vector<T, height>, width> array);

  ~Matrix()                         = default;
  Matrix<T, width, height> &operator=(const Matrix<T, width, height> &v);
  [[nodiscard]] bool        operator!=(const Matrix<T, width, height> &v) const;
  [[nodiscard]] bool        operator==(const Matrix<T, width, height> &v) const;
  [[nodiscard]] bool        operator>(const Matrix<T, width, height> &v) const;
  [[nodiscard]] bool        operator<(const Matrix<T, width, height> &v) const;
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool                     operator>=(const Matrix<T, width, height> &v) const;
  [[nodiscard]] bool                     operator<=(const Matrix<T, width, height> &v) const;
  [[nodiscard]] Vector<T, height> &      operator[](std::size_t);
  [[nodiscard]] const Vector<T, height> &operator[](std::size_t) const;
  [[nodiscard]] Vector<T, height> &      operator()(std::size_t);
  [[nodiscard]] Matrix<T, width, height> operator+(const Matrix<T, width, height> &v) const;
  [[nodiscard]] Matrix<T, width, height> operator-(const Matrix<T, width, height> &v) const;
  [[nodiscard]] Matrix<T, width, height> operator-(const Vector<T, height> &v) const;
  [[nodiscard]] Matrix<T, width, height> operator*(const Matrix<T, width, height> &v) const;
  [[nodiscard]] Matrix<T, width, height> operator*(const int &v) const;
  [[nodiscard]] Vector<T, height>        operator*(const Vector<T, width> &v) const;
  [[nodiscard]] Matrix<T, width, height> operator*(const float &v) const;
  [[nodiscard]] static Matrix<T, width, height> mix(const Matrix<T, width, height> & a, const Matrix<T, width, height> &b, const float & c);
  [[nodiscard]] Matrix<T, height, width> transpose() const;
  [[nodiscard]] Matrix<T, height, width> inverse();
  [[nodiscard]] size_t hash() const;
  [[nodiscard]] T determinant();
  void getCofactor(Matrix<T, width, height> &tmp, int p, int q);
  [[nodiscard]] Matrix<T, width, height> operator^(const Matrix<T, width, height> &v) const;
  Matrix<T, width, height> &             operator+=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, height> &             operator-=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, height> &             operator*=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
};

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height>::Matrix() : m_matrix{} {}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height>::Matrix(const std::array<std::array<T, height>, width> &array) : m_matrix{} {
  for (int i = 0; i < width; ++i)
    m_matrix[i] = Vector<T, height>{array[i]};
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height>::Matrix(const std::vector<std::vector<T>> &array) : m_matrix{} {
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
inline bool Matrix<T, width, height>::operator!=(const Matrix<T, width, height> &v) const {
  return m_matrix != v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator==(const Matrix<T, width, height> &v) const {
  return m_matrix == v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator>(const Matrix<T, width, height> &v) const {
  return m_matrix > v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator<(const Matrix<T, width, height> &v) const {
  return m_matrix < v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator>=(const Matrix<T, width, height> &v) const {
  return m_matrix >= v.m_matrix;
}

template <class T, std::size_t width, std::size_t height>
inline bool Matrix<T, width, height>::operator<=(const Matrix<T, width, height> &v) const {
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
inline Matrix<T, width, height> Matrix<T, width, height>::operator+(const Matrix<T, width, height> &v) const {
  return Matrix<T, width, height>{m_matrix + v.m_matrix};
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator-(const Matrix<T, width, height> &v) const {
  return Matrix<T, width, height>{m_matrix - v.m_matrix};
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator*(const Matrix<T, width, height> &v) const {
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
inline Matrix<T, width, height> Matrix<T, width, height>::operator^(const Matrix<T, width, height> &v) const {
  return Matrix<T, width, height>{m_matrix ^ v.m_matrix};
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> &Matrix<T, width, height>::operator+=(const Matrix<T, width, height> &v) {
  m_matrix += v.m_matrix;
  return *this;
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> &Matrix<T, width, height>::operator-=(const Matrix<T, width, height> &v) {
  m_matrix -= v.m_matrix;
  return *this;
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> &Matrix<T, width, height>::operator*=(const Matrix<T, width, height> &v) {
  if (width != height)
    throw std::runtime_error{"cannot multiply a n*m matrix by a n*m matrix only n*n matrices by n*n matrices"};
  *this = *this * v;  // we need to do a copy of *this to use it in the mul
  return *this;
}

template <class T, std::size_t width, std::size_t height>
Matrix<T, width, height>::Matrix(Vector<Vector<T, height>, width> array) : m_matrix{array} {}

template <class T, std::size_t width, std::size_t height>
Matrix<T, width, height> Matrix<T, width, height>::operator*(const int &v) const {
  return Matrix<T, width, height>{m_matrix * v};
}
template <class T, std::size_t width, std::size_t height>
Matrix<T, width, height> Matrix<T, width, height>::operator*(const float &v) const {
  Vector<Vector<T, height>, width> tmp = m_matrix * v;
  return Matrix<T, width, height>{tmp};
}
template <class T, std::size_t width, std::size_t height>
Matrix<T, height, width> Matrix<T, width, height>::transpose() const {
  Matrix<T, height, width> ret{};
  for (int i = 0; i < height; ++i)
    for (int j = 0; j < width; ++j)
      ret[i][j] = m_matrix[j][i];
  return ret;
}
template <class T, std::size_t width, std::size_t height>
const Vector<T, height> &Matrix<T, width, height>::operator[](std::size_t i) const {
  return m_matrix[i];
}

template <class T, std::size_t width, std::size_t height>
Vector<T, height> Matrix<T, width, height>::operator*(const Vector<T, width> &v) const {
  Vector<T, height> ret{};
  for (int i = 0; i < height; ++i) {
    ret[i] = 0.0f;
    for (int j = 0; j < width; ++j)
      ret[i] += m_matrix[i][j] * ret[j];
  }
  return ret;
}
template <class T, std::size_t width, std::size_t height>
T Matrix<T, width, height>::determinant() {
  if (width != height)
    return -1;
  T D = 0; // Initialize result

  //  Base case : if matrix contains single element
  if (width == 1)
    return this[0][0];

  Matrix<T, width, height> temp{}; // To store cofactors

  int sign = 1; // To store sign multiplier

  // Iterate for each element of first row
  for (int f = 0; f < width; f++)
  {
    // Getting Cofactor of mat[0][f]
    getCofactor(temp, 0, f);
    D += sign * m_matrix[0][f] * determinantOfMatrix(temp, width - 1);
    // terms are to be added with alternate sign
    sign = -sign;
  }

  return D;
}
template <class T, std::size_t width, std::size_t height>
void Matrix<T, width, height>::getCofactor(Matrix<T, width, height> &tmp, int p, int q) {
  int i = 0, j = 0;

  // Looping for each element of the matrix
  for (int row = 0; row < height; row++)
  {
    for (int col = 0; col < width; col++)
    {
      //  Copying into temporary matrix only those
      //  element which are not in given row and
      //  column
      if (row != p && col != q) {
        tmp[i][j++] = (*this)[row][col];

        // Row is filled, so increase row index and
        // reset col index
        if (j == width - 1)
        {
          j = 0;
          i++;
        }
      }
    }
  }
}
template <class T, std::size_t width, std::size_t height>
Matrix<T, height, width> Matrix<T, width, height>::inverse() {
  return *this * (1/determinant());
}
template <class T, std::size_t width, std::size_t height>
Matrix<T, width, height> Matrix<T, width, height>::mix(const Matrix<T, width, height> &a, const Matrix<T, width, height> &b, const float &c) {
  if (width != height)
    throw std::runtime_error{"cannot mix if height != width"};
  return a * (1.0f - c) + b * c;
}
template <class T, std::size_t width, std::size_t height>
Matrix<T, width, height> Matrix<T, width, height>::operator-(const Vector<T, height> &v) const {
  Matrix<T, width, height>ret{*this};
  for (auto &vec: ret.m_matrix)
    ret -= v;
  return ret;
}
template <class T, std::size_t width, std::size_t height>
size_t Matrix<T, width, height>::hash() const {
  size_t seed = 0;
  for (int i = 0; i < width; ++i)
    seed ^= m_matrix[i].hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  return seed;
}

template <class T>
class Matrix4 : public Matrix<T, 4, 4> {
public:
  explicit Matrix4();
  explicit Matrix4(T x, T y = 1.0f, T z = 1.0f, T w = 1.0f);  // identity multiplied by vector [a, b, c, d]
  explicit Matrix4(const std::array<std::array<T, 4>, 4> &array);
  explicit Matrix4(const std::vector<std::vector<T>> &array);
  Matrix4(const Matrix<T, 4, 4> &v);
  [[nodiscard]] static Matrix4<T> lookAt(const Vector<T, 3> &eye, const Vector<T, 3> &center, const Vector<T, 3> &up);
  [[nodiscard]] static Matrix4<T> perspective(T angle, T ratio, T near, T far);
  [[nodiscard]] Matrix4<T>        scale(T a, T b, T c);
  [[nodiscard]] static Matrix4<T> translate(const Vector3<T> &vec);
  [[nodiscard]] static Matrix4<T> perspectiveRH(T fovy, T aspect, T near, T far);
  [[nodiscard]] Vector3f getTranslation() const;
  void setTranslation(const Vector3<T> &vec);
};

template <class T>
Matrix4<T> Matrix4<T>::lookAt(const Vector<T, 3> &eye, const Vector<T, 3> &center, const Vector<T, 3> &up) {
  T X, y = up, z = eye - center, XDotEye, yDotEye, zDotEye;
  z.normalize();
  X = y.cross(z);
  y = z.cross(X);
  X.normalize();
  y.normalize();
  return Matrix4<T>({{{X[0], X[1], X[2], -X.dot(eye)}, {y[0], y[1], y[2], -y.dot(eye)}, {z[0], z[1], z[2], -z.dot(eye)}, {0, 0, 0, 1.0f}}});
}
template <class T>
Matrix4<T> Matrix4<T>::perspective(T angle, T ratio, T near, T far) {
  float tan_half_angle = tanf(angle / 2);
  return Matrix4<T>{{{1 / (ratio * tan_half_angle), 0, 0, 0}, {0, 1 / tan_half_angle, 0, 0}, {0, 0, -(far + near) / (far - near), -(2 * far * near) / (far - near)}, {0, 0, -1, 0}}};
}
template <class T>
Matrix4<T>::Matrix4(const std::array<std::array<T, 4>, 4> &array) : Matrix<T, 4, 4>(array) {}
template <class T>
Matrix4<T>::Matrix4(const std::vector<std::vector<T>> &array) : Matrix<T, 4, 4>(array) {}
template <class T>
Matrix4<T>::Matrix4() : Matrix<T, 4, 4>() {}
template <class T>
Matrix4<T>::Matrix4(T x, T y, T z, T w) : Matrix<T, 4, 4>{} {
  this->m_matrix[0][0] = x;
  this->m_matrix[1][1] = y;
  this->m_matrix[2][2] = z;
  this->m_matrix[3][3] = w;
}
template <class T>
Matrix4<T> Matrix4<T>::scale(T a, T b, T c) {
  return *this * Matrix4<T>{a, b, c};
}
template <class T>
Matrix4<T> Matrix4<T>::translate(const Vector3<T> &vec) {
  Matrix4<T> ret{1};
  ret[3][0] = vec[0];
  ret[3][1] = vec[1];
  ret[3][2] = vec[2];
  return ret;
}

template <class T>
void Matrix4<T>::setTranslation(const Vector3<T> &vec) {
  this->m_matrix[3][0] = vec[0];
  this->m_matrix[3][1] = vec[1];
  this->m_matrix[3][2] = vec[2];
}

template <class T>
Vector3f Matrix4<T>::getTranslation() const {
  return Vector3f {this->m_matrix[3][0], this->m_matrix[3][1], this->m_matrix[3][2]};
}

template <class T>
Matrix4<T> Matrix4<T>::perspectiveRH(T fovy, T aspect, T near, T far) {
  assert(abs(aspect - std::numeric_limits<T>::epsilon()) > static_cast<T>(0));

  T const tanHalfFovy = tan(fovy / static_cast<T>(2));

  Matrix4<T> result(0);  // when {0} error reference might be null
  result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
  result[1][1] = static_cast<T>(1) / (tanHalfFovy);
  result[2][2] = far / (near - far);
  result[2][3] = -static_cast<T>(1);
  result[3][2] = -(far * near) / (far - near);
  return result;
}
template <class T>
Matrix4<T>::Matrix4(const Matrix<T, 4, 4> &v): Matrix<T,4,4>(v) {}
