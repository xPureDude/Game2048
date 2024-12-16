#pragma once

#include <map>
#include "SparseSet.hpp"

template <typename ComponentT>
class Registry
{
public:
private:
    std::map<ComponentID, void*> m_components;
    std::map<SystemID, void*> m_systems;
    std::map<ResourceID, void*> m_resources;
};