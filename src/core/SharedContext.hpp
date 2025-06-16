#pragma once

#include <functional>
#include <map>
#include <typeindex>
#include <vector>

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
    static SharedContext* Instance();

    ~SharedContext();

    template <typename T>
    T* Emplace();

    template <typename T, typename... Args>
    T* Emplace(Args... args);

    template <typename T>
    void Release();

    template <typename T>
    T* Get();

    void ReleaseAll();

private:
    static SharedContext s_obj;
    std::vector<Context*> m_ctxList;
    std::map<std::type_index, Context> m_ctxs;
};

template <typename T>
inline T* SharedContext::Emplace()
{
    auto idx = std::type_index(typeid(T));
    if (m_ctxs.find(idx) == m_ctxs.end())
    {
        Context ctx;
        ctx.m_obj = (void*)(new T);
        ctx.m_deletor = [](void* obj) { delete static_cast<T*>(obj); };
        m_ctxs.emplace(idx, ctx);
        m_ctxList.push_back(&m_ctxs[idx]);
    }
    return static_cast<T*>(m_ctxs[idx].m_obj);
}

template <typename T, typename... Args>
inline T* SharedContext::Emplace(Args... args)
{
    auto idx = std::type_index(typeid(T));
    if (m_ctxs.find(idx) == m_ctxs.end())
    {
        Context ctx;
        ctx.m_obj = (void*)(new T(args...));
        ctx.m_deletor = [](void* obj) { delete static_cast<T*>(obj); };
        m_ctxs.emplace(idx, ctx);
        m_ctxList.push_back(&m_ctxs[idx]);
    }
    return static_cast<T*>(m_ctxs[idx].m_obj);
}

template <typename T>
inline void SharedContext::Release()
{
    auto idx = std::type_index(typeid(T));
    auto it = m_ctxs.find(idx);
    if (it != m_ctxs.end())
    {
        it->second.Release();
        m_ctxs.erase(it);

        for (auto iter = m_ctxList.begin(); iter != m_ctxList.end(); ++iter)
        {
            if (*iter == &(it->second))
            {
                m_ctxList.erase(iter);
                break;
            }
        }
    }
}

template <typename T>
inline T* SharedContext::Get()
{
    auto idx = std::type_index(typeid(T));
    return static_cast<T*>(m_ctxs[idx].m_obj);
}