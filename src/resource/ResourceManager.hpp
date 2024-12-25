#pragma once

#include "../pch.hpp" // IWYU pragma: keep

struct ResourceHead
{
    std::string m_filePath;
    int m_fromOffset;
    int m_toOffset;
    union {
        sf::Vector2i m_size;
    };
};

template <typename T>
struct ResourceInfo
{
    ResourceInfo();
    ~ResourceInfo();

    T* Get();
    bool Load();
    void Release();

    std::string m_filePath;
    int m_count;
    T* m_res;
};

template <typename T>
class ResourceManager
{
public:
    ResourceManager()
    {
    }

    virtual ~ResourceManager()
    {
        for (auto& info : m_resources)
        {
            info.second.Release();
        }
    }

    virtual bool LoadResourceInfo(const std::string& file) = 0;

    T* GetResource(const std::string& name)
    {
        auto itInfo = m_resources.find(name);
        if (itInfo == m_resources.end())
            return nullptr;

        return itInfo->second.Get();
    }

    void ReleaseResource(const std::string& name)
    {
        auto itInfo = m_resources.find(name);
        if (itInfo != m_resources.end())
        {
            itInfo->second.Release();
        }
    }

    void InsertResourceInfo(const std::string& name, ResourceInfo<T> info)
    {
        m_resources.emplace(name, info);
    }

protected:
    bool _ReLoadReource(const std::string& name)
    {
        return m_resources[name].Load();
    }

protected:
    std::unordered_map<std::string, ResourceInfo<T>> m_resources;
};

template <typename T>
ResourceInfo<T>::ResourceInfo()
    : m_count(0),
      m_res(nullptr)
{
}

template <typename T>
ResourceInfo<T>::~ResourceInfo()
{
    if (m_res != nullptr)
        delete m_res;
}

template <typename T>
T* ResourceInfo<T>::Get()
{
    if (m_res == nullptr)
    {
        if (Load() == true)
        {
            m_count++;
            return m_res;
        }
        else
        {
            return nullptr;
        }
    }
    m_count++;
    return m_res;
}

template <typename T>
bool ResourceInfo<T>::Load()
{
    if (m_res != nullptr)
        return false;
    m_res = new T;
    if (m_res->loadFromFile(m_filePath) == false)
        return false;
    m_count = 0;
    return true;
}

template <typename T>
void ResourceInfo<T>::Release()
{
    --m_count;
    if (m_count <= 0 && m_res != nullptr)
    {
        delete m_res;
        m_res = nullptr;
    }
}