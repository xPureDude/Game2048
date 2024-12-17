#pragma once

#include <typeindex>
#include <map>
#include <vector>

class SharedContext
{
public:
    SharedContext() {}
    ~SharedContext() 
    {
        for (auto& idx : m_list)
        {
            delete m_ctx[idx];
        }
    }

    template <typename T>
    T* Emplace()
    {
        auto idx = std::type_index(typeid(T));
        if (m_ctx.find(idx) == m_ctx.end())
        {
            T* obj = new T;
            m_ctx.emplace(idx, (void*)obj);
            m_list.push_back(idx);
        }
        return static_cast<T*>(m_ctx[idx]);
    }

    template <typename T, typename... Args>
    T* Emplace(Args... args)
    {
        auto idx = std::type_index(typeid(T));
        if (m_ctx.find(idx) == m_ctx.end())
        {
            T* obj = new T(std::forward(args...));
            m_ctx.emplace(idx, (void*)obj);
            m_list.push_back(idx);
        }
        return static_cast<T*>(m_ctx[idx]);
    }

    template <typename T>
    T* Get()
    {
        auto idx = std::type_index(typeid(T));
        return static_cast<T*>(m_ctx[idx]);
    }

private:
    std::map<std::type_index, void*> m_ctx;
    std::vector<std::type_index> m_list;
};
