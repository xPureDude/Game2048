#pragma once

#include <vector>

#include "EcsConfig.hpp"

class SparseSet
{
public:
    bool HasEntity(Entity entity);

    void AddEntity(Entity entity);

    void DelEntity(Entity entity);

    const std::vector<Entity>& GetEntities();
    std::size_t GetPackedIndex(Entity entity);
    std::size_t GetSize();

private:
    std::vector<Entity> m_sparse;
    std::vector<Entity> m_packed;
};