#pragma once

#include <memory>

template<typename T>
class ICollisionShape
{
    public:
        virtual ~ICollisionShape() = default;
        [[nodiscard]] virtual bool operator==(const T&) const = 0;
};