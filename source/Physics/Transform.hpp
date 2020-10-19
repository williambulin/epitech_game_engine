#pragma once

class Transform {
public:
  Mat4 m_modelMatrix{};

public:
  explicit Transform();

  bool operator==(const Transform &transform);
};