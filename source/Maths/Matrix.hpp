#include <array>
#include <vector>
#include "Vectors.hpp"

template <class T, std::size_t width, std::size_t height>
class Matrix final {
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
  [[nodiscard]] Vector<T, height> &       operator[](std::size_t);
  [[nodiscard]] Vector<T, height> &       operator()(std::size_t);
  [[nodiscard]] Matrix<T, width, height> operator+(const Matrix<T, width, height> &v);
  [[nodiscard]] Matrix<T, width, height> operator-(const Matrix<T, width, height> &v);
  [[nodiscard]] Matrix<T, width, height> operator*(const Matrix<T, width, height> &v);
  [[nodiscard]] Matrix<T, width, height> operator*(const int &v);
  [[nodiscard]] Matrix<T, width, height> operator*(const float &v);
  [[nodiscard]] Matrix<T, height, width> transpose();
  [[nodiscard]] Matrix<T, width, height> operator/(const Matrix<T, width, height> &v);
  [[nodiscard]] Matrix<T, width, height> operator^(const Matrix<T, width, height> &v);
  Matrix<T, width, height>               operator+=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, height>               operator-=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, height>               operator*=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, height>               operator/=(const Matrix<T, width, height> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
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
    std::copy_n(v.m_matrix[i].begin(), height, m_matrix[i].begin());
  }
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
  return Matrix<T, width, height>{m_matrix * v.m_matrix};
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator/(const Matrix<T, width, height> &v) {
  return Matrix<T, width, height>{m_matrix / v.m_matrix};
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
  m_matrix *= v.m_matrix;
  return *this;
}

template <class T, std::size_t width, std::size_t height>
inline Matrix<T, width, height> Matrix<T, width, height>::operator/=(const Matrix<T, width, height> &v) {
  m_matrix /= v.m_matrix;
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
