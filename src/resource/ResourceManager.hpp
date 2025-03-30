#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "SFML/Graphics.hpp" // IWYU pragma: keep

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

    std::shared_ptr<T> Get();

    std::string m_filePath;
    std::shared_ptr<T> m_res;

private:
    bool _Load();
};

template <typename T>
ResourceInfo<T>::ResourceInfo()
    : m_res(nullptr)
{
}

template <typename T>
std::shared_ptr<T> ResourceInfo<T>::Get()
{
    if (!m_res)
    {
        if (_Load())
        {
            return m_res;
        }
        else
        {
            return nullptr;
        }
    }
    return m_res;
}

template <typename T>
bool ResourceInfo<T>::_Load()
{
    if (m_res != nullptr)
        return false;
    m_res = std::make_shared<T>();
    if (m_res->loadFromFile(m_filePath) == false)
        return false;
    return true;
}

template <>
bool ResourceInfo<sf::Font>::_Load();

template <typename T>
class ResourceManager
{
public:
    ResourceManager() = default;
    virtual ~ResourceManager() {}

    virtual bool LoadResourceInfoFromFile(const std::string& file) = 0;

    void SlowUpdate();

    bool LoadResourceInfo(const std::string& name, const std::string& path);
    std::shared_ptr<T> RequestResource(const std::string& name);

protected:
    std::unordered_map<std::string, ResourceInfo<T>> m_resources;
};

template <typename T>
void ResourceManager<T>::SlowUpdate()
{
    for (auto& item : m_resources)
    {
        if (item.second.m_res.use_count() == 1)
        {
            item.second.m_res = nullptr;
        }
    }
}

template <typename T>
bool ResourceManager<T>::LoadResourceInfo(const std::string& name, const std::string& path)
{
    auto result = m_resources.try_emplace(name, ResourceInfo<T>());
    if (result.second)
    {
        result.first->second.m_filePath = path;
    }
    return result.second;
}

template <typename T>
std::shared_ptr<T> ResourceManager<T>::RequestResource(const std::string& name)
{
    auto itInfo = m_resources.find(name);
    if (itInfo == m_resources.end())
        return nullptr;

    return itInfo->second.Get();
}