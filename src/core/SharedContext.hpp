#pragma once

#include "../pch.hpp" // IWYU pragma: keep

using Deletor = std::function<void(void*)>;

class SharedContext
{
    struct Context
    {
        void Release() { m_deletor(m_obj); }
        void* m_obj;
        Deletor m_deletor;
    };

    SharedContext() = default;

public:
    static SharedContext& Instance();

    ~SharedContext();

    template <typename T>
    T* Emplace(Deletor deletor);

    template <typename T, typename... Args>
    T* Emplace(Deletor deletor, Args... args);

    template <typename T>
    void Release();

    template <typename T>
    T* Get();

private:
    static SharedContext s_obj;
    std::map<std::type_index, Context> m_ctxs;
};

template <typename T>
T* SharedContext::Emplace(Deletor deletor)
{
    auto idx = std::type_index(typeid(T));
    if (m_ctxs.find(idx) == m_ctxs.end())
    {
        Context ctx;
        ctx.m_obj = (void*)(new T);
        ctx.m_deletor = deletor;
        m_ctxs.emplace(idx, ctx);
    }
    return static_cast<T*>(m_ctxs[idx].m_obj);
}

template <typename T, typename... Args>
T* SharedContext::Emplace(Deletor deletor, Args... args)
{
    auto idx = std::type_index(typeid(T));
    if (m_ctxs.find(idx) == m_ctxs.end())
    {
        Context ctx;
        ctx.m_obj = (void*)(new T(args...));
        ctx.m_deletor = deletor;
        m_ctxs.emplace(idx, ctx);
    }
    return static_cast<T*>(m_ctxs[idx].m_obj);
}

template <typename T>
void SharedContext::Release()
{
    auto idx = std::type_index(typeid(T));
    auto it = m_ctxs.find(idx);
    if (it != m_ctxs.end())
    {
        it->second.Release();
        m_ctxs.erase(it);
    }
}

template <typename T>
T* SharedContext::Get()
{
    auto idx = std::type_index(typeid(T));
    return static_cast<T*>(m_ctxs[idx].m_obj);
}
