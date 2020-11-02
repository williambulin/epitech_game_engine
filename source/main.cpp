#include <iostream>
#include "Core.hpp"
#include "Maths/Matrix.hpp"
#include "Maths/Quaternion.hpp"

template <class T, std::size_t width, std::size_t height>
void printMatrix(Matrix<T, width, height> m) {
  std::cout << "Matrix " << width << "x" << height << std::endl;
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; ++j) {
      std::cout << m[i][j] << "\t";
    }
    std::cout << std::endl;
  }
}

template <class T, std::size_t size>
void printVector(Vector<T, size> v) {
  std::cout << "Vector<" << size << "> {";
  for (int i = 0; i < size; ++i)
    std::cout << v[i]<< " ";
  std::cout << "}" << std::endl;
}

int main(int argc, char **argv, char **env) {
  Matrix<int, 3, 3> m{{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
  Matrix4<float>    n{std::array<std::array<float, 4>, 4>{
  std::array<float, 4>{1.0f, 2.0f, 3.0f, 4.0f},
  std::array<float, 4>{5.0f, 6.0f, 7.0f, 8.0f},
  std::array<float, 4>{9.0f, 10.0f, 11.0f, 12.0f},
  std::array<float, 4>{13.0f, 14.0f, 15.0f, 16.0f}
  }};
  Vector2<float> vec = Vector2<float>{std::array<float, 2>{1, 2}};
  printMatrix(Matrix4<float>::mix(n, n, 3));
  printVector(vec);
  vec = vec;
 /* printMatrix(n.transpose());
  printMatrix(m * 3);
  printMatrix(m * m);
  printMatrix(m *= m);
  printMatrix(n * 2.0f);
  printMatrix(Quaternion::fromMatrix(n).toRotationMatrix());*/
  //  auto p = m * n;
  //  std::cout << p[0][0] << std::endl;
  Core app{argc, argv, env};
  return app.run();
}
