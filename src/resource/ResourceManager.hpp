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
    ResourceManager() = default;

    virtual ~ResourceManager();

    virtual bool LoadResourceInfo(const std::string& file) = 0;

    bool LoadFontFromFile(const std::string& name, const std::string& path);

    T* GetResource(const std::string& name);

    void ReleaseResource(const std::string& name);

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

template <>
bool ResourceInfo<sf::Font>::Load();

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

template <typename T>
ResourceManager<T>::~ResourceManager()
{
    for (auto& info : m_resources)
    {
        info.second.Release();
    }
}

template <typename T>
bool ResourceManager<T>::LoadFontFromFile(const std::string& name, const std::string& path)
{
    auto result = m_resources.try_emplace(name, ResourceInfo<T>());
    if (result.second)
    {
        result.first->second.m_filePath = path;
    }
    return result.second;
}

template <typename T>
T* ResourceManager<T>::GetResource(const std::string& name)
{
    auto itInfo = m_resources.find(name);
    if (itInfo == m_resources.end())
        return nullptr;

    return itInfo->second.Get();
}

template <typename T>
void ResourceManager<T>::ReleaseResource(const std::string& name)
{
    auto itInfo = m_resources.find(name);
    if (itInfo != m_resources.end())
    {
        itInfo->second.Release();
    }
}
