#include "SparseSet.hpp"

bool SparseSet::HasEntity(Entity entity)
{
    return entity < m_sparse.size() && m_sparse[entity] != NULL_ENTITY;
}

void SparseSet::AddEntity(Entity entity)
{
    if (HasEntity(entity))
        return;

    if (entity >= m_sparse.size())
    {
        m_sparse.resize(entity + 1, NULL_ENTITY);
    }

    m_sparse[entity] = m_packed.size();
    m_packed.push_back(entity);
}

void SparseSet::DelEntity(Entity entity)
{
    if (!HasEntity(entity))
        return;

    m_packed[m_sparse[entity]] = m_packed.back();
    m_sparse[m_packed.back()] = m_sparse[entity];
    m_packed.pop_back();
    m_sparse[entity] = NULL_ENTITY;
}

const std::vector<Entity>& SparseSet::GetEntities()
{
    return m_packed;
}

std::size_t SparseSet::GetPackedIndex(Entity entity)
{
    if (!HasEntity(entity))
        return NULL_ENTITY;

    return m_sparse[entity];
}

std::size_t SparseSet::GetSize()
{
    return m_packed.size();
}