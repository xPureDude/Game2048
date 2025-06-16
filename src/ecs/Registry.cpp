#include "Registry.hpp"

Entity Registry::CreateEntity()
{
    return ++m_entityCount;
}

void Registry::DestroyEntity(Entity entity)
{
    for (auto& [_, storage] : m_componentStorages)
    {
        if (storage->Contains(entity))
            storage->Del(entity);
    }
}