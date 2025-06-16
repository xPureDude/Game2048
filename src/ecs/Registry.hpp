#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "ComponentStorage.hpp"
#include "EcsConfig.hpp"

class Registry
{
public:
    Entity CreateEntity();
    void DestroyEntity(Entity entity);

    template <typename ComponentT, typename... Args>
    void AddComponent(Entity entity, Args&&... args);

    template <typename ComponentT>
    void DelComponent(Entity entity);

    template <typename ComponentT>
    ComponentT* GetComponent(Entity entity);

    template <typename ComponentT>
    ComponentStorage<ComponentT>* GetStorage();

private:
    Entity m_entityCount{0};
    std::unordered_map<std::type_index, std::unique_ptr<IComponentStorage>> m_componentStorages;
};

template <typename ComponentT, typename... Args>
void Registry::AddComponent(Entity entity, Args&&... args)
{
    auto id = ComponentID<ComponentT>::ID();
    if (!m_componentStorages.contains(id))
    {
        m_componentStorages[id] = std::make_unique<ComponentStorage<ComponentT>>();
    }
    auto storage = static_cast<ComponentStorage<ComponentT>*>(m_componentStorages.at(id).get());
    storage->Add(entity, std::forward<Args>(args)...);
}

template <typename ComponentT>
void Registry::DelComponent(Entity entity)
{
    auto id = ComponentID<ComponentT>::ID();
    if (!m_componentStorages.contains(id))
    {
        return;
    }
    m_componentStorages.at(id)->Del(entity);
}

template <typename ComponentT>
ComponentT* Registry::GetComponent(Entity entity)
{
    auto id = ComponentID<ComponentT>::ID();
    if (!m_componentStorages.contains(id))
    {
        return nullptr;
    }
    auto storage = static_cast<ComponentStorage<ComponentT>*>(m_componentStorages.at(id).get());
    return storage->Get(entity);
}

template <typename ComponentT>
ComponentStorage<ComponentT>* Registry::GetStorage()
{
    auto id = ComponentID<ComponentT>::ID();
    if (!m_componentStorages.contains(id))
    {
        return nullptr;
    }

    auto storage = static_cast<ComponentStorage<ComponentT>*>(m_componentStorages.at(id).get());
    return storage;
}