#include <memory>

template<typename T>
class ICollisionSystem
{
    public:
        virtual ~ICollisionSystem() {}
        [[nodiscard]] virtual bool Overlap(const T&) = 0;
        [[nodiscard]] virtual bool operator==(const T&) = 0;
};