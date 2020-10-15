#include <array>

template <class T, std::size_t width, std::size_t heigth>
class Matrix final {
private:
  std::array<std::array<T, heigth>, width> m_matrix;

public:
  explicit Matrix();
  explicit Matrix(std::array<std::array<T, heigth>, width> array);
  explicit Matrix(std::vector<std::vector<T>> array);

  ~Matrix()                  = default;
  void               operator=(const Matrix<T, width, heigth> &v);
  [[nodiscard]] bool operator!=(const Matrix<T, width, heigth> &v);
  [[nodiscard]] bool operator==(const Matrix<T, width, heigth> &v);
  [[nodiscard]] bool operator>(const Matrix<T, width, heigth> &v);
  [[nodiscard]] bool operator<(const Matrix<T, width, heigth> &v);
  // the 2 functions above return true only if both members agree with the condition
  // Examples: this > v is true if this = {1, 1} and v = {0, 0} but if this = {1, 0} and v = {0, 1} this will be false
  // this < v will be true if this = {0, 0} and v = {1, 1} but if this = {1, 0} and v = {0, 1} this will be false
  [[nodiscard]] bool                     operator>=(const Matrix<T, width, heigth> &v);
  [[nodiscard]] bool                     operator<=(const Matrix<T, width, heigth> &v);
  [[nodiscard]] T &                      operator[](std::size_t);
  [[nodiscard]] T &                      operator()(std::size_t);
  [[nodiscard]] Matrix<T, width, heigth> operator+(const Matrix<T, width, heigth> &v);
  [[nodiscard]] Matrix<T, width, heigth> operator-(const Matrix<T, width, heigth> &v);
  [[nodiscard]] Matrix<T, width, heigth> operator*(const Matrix<T, width, heigth> &v);
  [[nodiscard]] Matrix<T, width, heigth> operator/(const Matrix<T, width, heigth> &v);
  [[nodiscard]] Matrix<T, width, heigth> operator^(const Matrix<T, width, heigth> &v);
  Matrix<T, width, heigth>               operator+=(const Matrix<T, width, heigth> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, heigth>               operator-=(const Matrix<T, width, heigth> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, heigth>               operator*=(const Matrix<T, width, heigth> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
  Matrix<T, width, heigth>               operator/=(const Matrix<T, width, heigth> &v);  // This function return a reference to itself to be able to chain itself or with other but the return value may not be used.
};

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth>::Matrix() : m_matrix{} {}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth>::Matrix(std::array<std::array<T, heigth>, width> array) : m_matrix{} {
  for (int i = 0; i < width; ++i) {
    std::copy_n(array[i].begin(), size, m_matrix[i].begin());
  }
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth>::Matrix(std::vector<std::vector<T>> array) {
  for (int i = 0; i < width; ++i) {
    std::copy_n(array[i].begin(), size, m_matrix[i].begin());
  }
}

template <class T, std::size_t width, std::size_t heigth>
inline void Matrix<T, width, heigth>::operator=(const Matrix<T, width, heigth> &v) {
  for (int i = 0; i < width; ++i) {
    std::copy_n(v.m_matrix[i].begin(), size, m_matrix[i].begin());
  }
}

template <class T, std::size_t width, std::size_t heigth>
inline bool Matrix<T, width, heigth>::operator!=(const Matrix<T, width, heigth> &v) {
  return m_matrix != v.m_matrix;
}

template <class T, std::size_t width, std::size_t heigth>
inline bool Matrix<T, width, heigth>::operator==(const Matrix<T, width, heigth> &v) {
  return m_matrix == v.m_matrix;
}

template <class T, std::size_t width, std::size_t heigth>
inline bool Matrix<T, width, heigth>::operator>(const Matrix<T, width, heigth> &v) {
  return m_matrix > v.m_matrix;
}

template <class T, std::size_t width, std::size_t heigth>
inline bool Matrix<T, width, heigth>::operator<(const Matrix<T, width, heigth> &v) {
  return m_matrix < v.m_matrix;
}

template <class T, std::size_t width, std::size_t heigth>
inline bool Matrix<T, width, heigth>::operator>=(const Matrix<T, width, heigth> &v) {
  return m_matrix >= v.m_matrix;
}

template <class T, std::size_t width, std::size_t heigth>
inline bool Matrix<T, width, heigth>::operator<=(const Matrix<T, width, heigth> &v) {
  return m_matrix <= v.m_matrix;
}

template <class T, std::size_t width, std::size_t heigth>
inline T &Matrix<T, width, heigth>::operator[](std::size_t i) {
  return m_matrix[i];
}

template <class T, std::size_t width, std::size_t heigth>
inline T &Matrix<T, width, heigth>::operator()(std::size_t i) {
  return m_matrix[i];
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth> Matrix<T, width, heigth>::operator+(const Matrix<T, width, heigth> &v) {
  Matrix<T, width, heigth> m{m_matrix};
  
  return m;
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth> Matrix<T, width, heigth>::operator-(const Matrix<T, width, heigth> &v) {
  return Matrix<T, width, heigth>();
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth> Matrix<T, width, heigth>::operator*(const Matrix<T, width, heigth> &v) {
  return Matrix<T, width, heigth>();
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth> Matrix<T, width, heigth>::operator/(const Matrix<T, width, heigth> &v) {
  return Matrix<T, width, heigth>();
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth> Matrix<T, width, heigth>::operator^(const Matrix<T, width, heigth> &v) {
  return Matrix<T, width, heigth>();
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth> Matrix<T, width, heigth>::operator+=(const Matrix<T, width, heigth> &v) {
  return Matrix<T, width, heigth>();
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth> Matrix<T, width, heigth>::operator-=(const Matrix<T, width, heigth> &v) {
  return Matrix<T, width, heigth>();
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth> Matrix<T, width, heigth>::operator*=(const Matrix<T, width, heigth> &v) {
  return Matrix<T, width, heigth>();
}

template <class T, std::size_t width, std::size_t heigth>
inline Matrix<T, width, heigth> Matrix<T, width, heigth>::operator/=(const Matrix<T, width, heigth> &v) {
  return Matrix<T, width, heigth>();
}
