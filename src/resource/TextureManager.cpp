#include "TextureManager.hpp"

#include "../common/Log.hpp"

bool TextureManager::LoadResourceInfo(const std::string& file)
{
    if (file.ends_with(".xml"))
    {
        return _LoadResourceInfoFromXml(file);
    }
    return false;
}

bool TextureManager::_LoadResourceInfoFromXml(const std::string& file)
{
    tinyxml2::XMLDocument xmlDoc;
    if (xmlDoc.LoadFile(file.c_str()) != tinyxml2::XML_SUCCESS)
    {
        ERR("TextureManager::_LoadResourceInfoFromXml, failed to load file: {}", file);
        return false;
    }

    auto resourceRoot = xmlDoc.RootElement();
    if (resourceRoot == nullptr)
        return false;

    tinyxml2::XMLElement* textureRoot = nullptr;
    for (auto e = resourceRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (strcmp(e->Value(), "Texture") == 0)
        {
            textureRoot = e;
            break;
        }
    }
    if (textureRoot == nullptr)
        return false;

    for (auto e = textureRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        std::string name = e->Attribute("name");
        ResourceInfo<sf::Texture> info;
        info.m_filePath = e->Attribute("filepath");
        m_resources.emplace(name, info);
    }

    return true;
}