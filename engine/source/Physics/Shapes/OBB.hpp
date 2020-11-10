#pragma once

#include "Extension/Language/Library.hpp"
#include "Maths/Vectors.hpp"
#include "Physics/ICollisionShape.hpp"
#include "Components/Transform.hpp"

class OBB final : public ICollisionShape {
public:
  DLLATTRIB explicit OBB(const ml::vec3 &min, const ml::vec3 &max) noexcept;
  DLLATTRIB explicit OBB(const OBB &second) noexcept;

  [[nodiscard]] auto getPoints(const ml::mat4 &transform, bool forceInvalidate = false) -> std::vector<ml::vec3>;  // Called by collide(...)

  void               setMin(const ml::vec3 &min) noexcept;
  [[nodiscard]] auto getMin() const noexcept -> ml::vec3;
  void               setMax(const ml::vec3 &max) noexcept;
  [[nodiscard]] auto getMax() const noexcept -> ml::vec3;

  [[nodiscard]] bool operator==(const OBB &second) const noexcept;

  ml::vec3 getLocalPosition() override;

private:
  ml::vec3              m_min{0.0f, 0.0f, 0.0f};
  ml::vec3              m_max{0.0f, 0.0f, 0.0f};
  ml::mat4              m_oldTransform{};
  std::vector<ml::vec3> m_pointsCache{};
};
