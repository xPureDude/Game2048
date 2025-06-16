#pragma once

#include "EcsConfig.hpp"
#include "SparseSet.hpp"

class IComponentStorage
{
public:
    virtual ~IComponentStorage() = default;

    virtual void Del(Entity entity) = 0;

    bool Contains(Entity entity) { return m_sparseSet.HasEntity(entity); }
    std::size_t GetSize() { return m_sparseSet.GetSize(); }
    const std::vector<Entity>& GetEntities() { return m_sparseSet.GetEntities(); }

protected:
    SparseSet m_sparseSet;
};

template <typename CompT>
class ComponentStorage : public IComponentStorage
{
public:
    virtual void Del(Entity entity) override
    {
        if (!m_sparseSet.HasEntity(entity))
            return;
        auto index = m_sparseSet.GetPackedIndex(entity);
        m_components[index] = std::move(m_components.back());
        m_components.pop_back();
        m_sparseSet.DelEntity(entity);
    }

    template <typename... Args>
    CompT& Add(Entity entity, Args&&... args)
    {
        if (m_sparseSet.HasEntity(entity))
            return;

        m_sparseSet.AddEntity(entity);
        m_components.emplace_back(std::forward<Args>(args)...);
        return m_components.back();
    }

    CompT* Get(Entity entity)
    {
        if (!m_sparseSet.HasEntity(entity))
            return nullptr;

        if (m_components.contains(entity))
            return &m_components[entity];
        return nullptr;
    }

    const std::vector<CompT>& GetComponents() { return m_components; }

private:
    std::vector<CompT> m_components;
};