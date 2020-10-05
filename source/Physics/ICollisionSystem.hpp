#pragma once

#include <memory>

template<typename T>
class ICollisionSystem
{
    public:
        virtual ~ICollisionSystem() = default;
        [[nodiscard]] virtual bool overlap(const T&) const = 0;
        [[nodiscard]] virtual bool operator==(const T&) const = 0;
};