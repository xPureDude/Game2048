#pragma once

#include "../pch.pch" // IWYU pragma: keep

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
    ResourceInfo()
        : m_count(0),
          m_res(nullptr)
    {
    }

    ~ResourceInfo()
    {
        if (m_res != nullptr)
            delete m_res;
    }

    T* Get()
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

    bool Load()
    {
        if (m_res != nullptr)
            return false;
        m_res = new T;
        if (m_res->loadFromFile(m_filePath) == false)
            return false;
        m_count = 0;
        return true;
    }
    void Release()
    {
        --m_count;
        if (m_count <= 0 && m_res != nullptr)
        {
            delete m_res;
            m_res = nullptr;
        }
    }

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