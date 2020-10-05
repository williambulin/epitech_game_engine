Add a [[nodiscard]] tag to functions that returns something so that the compiler throws a warning when the result is unused.

---

Mark constructors as explicit, example
```c++
explicit Vec3(...);
```

---

Specify whether the derivation is public or private, this will cause unexpected behavior.
```c++
class AABB : public ICollisionSystem<AABB> {
// ...
```

---

Use quotes to import local files.
While <> works, it's reserved for system imports.
```c++
#include "Maths/Vec3.hpp"
```

---

If you do not intend to inherit from this class later on (which shouldn't be the case for a non-generic vector class), please mark the class as final as such:
```c++
class Vec3 final {
// ...
```
---

It is preferable to separate member functions and constructors from member variables, even if they're both in the same space.
```c++
class Test final {
private: // Even if public, a separation makes the whole class more readable
  float m_variable{0.0f};

public:
  explicit Test();
};
```
Try to keep the member variables above the function definitions. This is important in cases where you need to refer to the member variable afterwards in the class itself. Example:
```c++
class Test final {
private: // Even if public, a separation makes the whole class more readable
  float m_variable{0.0f};

public:
  [[nodiscard]] auto getVariable() -> decltype(m_variable); // Here

  explicit Test();
};
```
---

A few guidelines when naming member variables:

- Respect the camelCase convention

- Member variables should start with m_ if they aren't short (example, x, y) and are only intended to be used within it's own class, so while fine in that case since you would probably use this class as a structure and directly access bottomLeft, it's just something to keep in mind

---

If you're not going to specify a body, use default.
```c++
virtual ~ICollisionSystem() = default; // instead of an empty body: {}
```

---

Either mark the function as an override or as final (which is final in that case since the class also is)
This keyword has to be applied on every virtual function that you override.
```c++
virtual void Test() = 0;
void Test() override;
void Test() final; // Useful if you want to derivate from it again but not allow the user to override the virtual fu
```

---

Member functions should be camelCase too:
```c++
[[nodiscard]] bool overlap(...) noexcept
```
The function should be marked as const since it does not modify anything within the class
```c++
[[nodiscard]] bool overlap(...) const noexcept
```

---

Use brackets instead of parenthesis to initialize variables and objects.
```c++
: topRight{topRight},
bottomLeft{bottomLeft}
```
Also try to have different names in the constructor than the member variables (hence the usefulness of m_)

---

Always make variable initialization explicit:
```c++
Vec3 topRight{}; // if nothing
VkImageView m_imageView{nullptr}; // For both pointers and smartptrs, initialize with nullptr
std::unique_ptr<int> m_testPtr{nullptr};
int m_volume{0);
float m_volume{0.0f};
double m_volume{0.0};
```

---

As a matter of reference, operator== will most likely always be const (they just compare and don't modify), so you should mark these as such
```c++
[[nodiscard]] bool operator==(...) const noexcept
```