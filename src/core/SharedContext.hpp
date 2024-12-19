#pragma once

#include "../pch.pch" // IWYU pragma: keep

#include <typeindex>


using Deletor = std::function<void(void*)>;

class SharedContext
{
    struct Context
    {
        void Release() { m_deletor(m_obj); }
        void* m_obj;
        Deletor m_deletor;
    };

public:
    SharedContext() {}
    ~SharedContext()
    {
        for (auto& it : m_ctx)
        {
            it.second.Release();
        }
    }

    template <typename T>
    T* Emplace(Deletor deletor)
    {
        auto idx = std::type_index(typeid(T));
        if (m_ctx.find(idx) == m_ctx.end())
        {
            Context ctx;
            ctx.m_obj = (void*)(new T);
            ctx.m_deletor = deletor;
            m_ctx.emplace(idx, ctx);
        }
        return static_cast<T*>(m_ctx[idx].m_obj);
    }

    template <typename T, typename... Args>
    T* Emplace(Deletor deletor, Args... args)
    {
        auto idx = std::type_index(typeid(T));
        if (m_ctx.find(idx) == m_ctx.end())
        {
            Context ctx;
            ctx.m_obj = (void*)(new T(args...));
            ctx.m_deletor = deletor;
            m_ctx.emplace(idx, ctx);
        }
        return static_cast<T*>(m_ctx[idx].m_obj);
    }

    template <typename T>
    void Release()
    {
        auto idx = std::type_index(typeid(T));
        auto it = m_ctx.find(idx);
        if (it != m_ctx.end())
        {
            it->second.Release();
            m_ctx.erase(it);
        }
    }

    template <typename T>
    T* Get()
    {
        auto idx = std::type_index(typeid(T));
        return static_cast<T*>(m_ctx[idx].m_obj);
    }

private:
    std::map<std::type_index, Context> m_ctx;
};
