#pragma once

#include <cstdint>
#include <typeindex>

using Entity = std::uint32_t;
constexpr Entity NULL_ENTITY = UINT32_MAX;

template <typename ComponentT>
struct ComponentID
{
    static std::type_index ID()
    {
        return std::type_index(typeid(ComponentT));
    }
};