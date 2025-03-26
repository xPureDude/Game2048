#pragma once

#include <map>

using ComponentID = std::uint32_t;
using SystemID = std::uint32_t;
using ResourceID = std::uint32_t;

template <typename ComponentT>
class Registry
{
public:
private:
    std::map<ComponentID, void*> m_components;
    std::map<SystemID, void*> m_systems;
    std::map<ResourceID, void*> m_resources;
};