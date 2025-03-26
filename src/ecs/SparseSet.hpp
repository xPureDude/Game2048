#pragma once

#include <array>
#include <tuple>
#include <vector>


template <typename T, uint32_t pageSize>
class SparseSet
{
    struct _Sparse
    {
        void Reset()
        {
            m_id = 0;
            m_data.Reset();
        }
        uint32_t m_id;
        T m_data;
    };

public:
    SparseSet()
        : m_nextSparseIndex(1)
    {
        m_sparse.resize(m_nextSparseIndex); // m_sparse[0] reserved;
    }

    ~SparseSet() {}

    bool CheckSparseByID(uint32_t id)
    {
        auto [page, index] = _TransferIndex(id);
        if (m_vec.size() <= page)
            return false;
        return m_vec[page][index] != 0;
    }

    T& GetSparseByID(uint32_t id)
    {
        auto [page, index] = _TransferIndex(id);
        uint32_t sparseIndex = m_vec[page][index];
        return m_sparse[sparseIndex].m_data;
    }

    T& AddSparseByID(uint32_t id)
    {
        auto [page, index] = _TransferIndex(id);
        if (m_vec.size() <= page)
            m_vec.resize(page + 1);

        if (m_sparse.size() == m_nextSparseIndex)
        {
            m_sparse.emplace_back();
        }
        m_sparse[m_nextSparseIndex].m_id = id;
        m_vec[page][index] = m_nextSparseIndex;
        ++m_nextSparseIndex;

        return m_sparse[m_nextSparseIndex - 1].m_data;
    }

    void DelSparseByID(uint32_t id)
    {
        if (CheckSparseByID(id) == false)
            return;

        auto [page, index] = _TransferIndex(id);
        uint32_t sparseIndex = m_vec[page][index];
        m_sparse[sparseIndex].Reset();
        m_vec[page][index] = 0;
        --m_nextSparseIndex;
        uint32_t otherID = m_sparse[m_nextSparseIndex].m_id;
        std::swap(m_sparse[sparseIndex], m_sparse[m_nextSparseIndex]);

        if (otherID > 0)
        {
            auto [oPage, oIndex] = _TransferIndex(otherID);
            m_vec[oPage][oIndex] = sparseIndex;
        }
    }

private:
    std::tuple<uint32_t, uint32_t> _TransferIndex(uint32_t id)
    {
        return {id / pageSize, id % pageSize};
    }

private:
    uint32_t m_nextSparseIndex;
    std::vector<_Sparse> m_sparse;
    std::vector<std::array<uint32_t, pageSize>> m_vec;
};