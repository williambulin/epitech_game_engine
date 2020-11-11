#pragma once

#include "Maths/Math.hpp"

namespace ml {
  namespace shader {
    struct vec3;
  }
}

struct ml::shader::vec3 {
  float x{0.0f};
  float y{0.0f};
  float z{0.0f};

  inline vec3() : x{0.0f}, y{0.0f}, z{0.0f} {}
  inline vec3(float n) : x{n}, y{n}, z{n} {}
  inline vec3(float _x, float _y, float _z) : x{_x}, y{_y}, z{_z} {}
  inline vec3(const ml::vec3 &vec) : x{vec.x}, y{vec.y}, z{vec.z} {}
};

namespace Components {
  class Light;
}

class Components::Light final {
public:
  enum class Type : std::uint32_t {
    Directional = 0,
    Point       = 1,
  };

  struct GLSLStruct {
    alignas(16) ml::shader::vec3 position{0.0f, 0.0f, 0.0f};
    alignas(16) ml::shader::vec3 direction{0.0f, -1.0f, 0.0f};
    alignas(16) ml::shader::vec3 color{1.0f, 1.0f, 1.0f};
    alignas(4) float strength{10.0f};
    alignas(4) std::uint32_t type{static_cast<std::uint32_t>(Type::Point)};
  };

public:
  ml::vec3 direction{0.0f, -1.0f, 0.0f};
  ml::vec3 color{1.0f, 1.0f, 1.0f};
  float    strength{10.0f};
  Type     type{Type::Point};

  [[nodiscard]] inline auto toGLSLStruct() -> GLSLStruct {
    return GLSLStruct{
    .direction = direction,
    .color     = color,
    .strength  = strength,
    .type      = static_cast<std::uint32_t>(type),
    };
  }
};
