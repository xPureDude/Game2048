#include "TextureManager.hpp"

#include "common/Log.hpp"

bool TextureManager::LoadResourceInfoFromFile(const std::string& file)
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

    tinyxml2::XMLElement* textureRoot = xmlDoc.RootElement();
    if (!textureRoot)
    {
        ERR("TextureManager::_LoadResourceInfoFromXml, RootElement is nullptr, file: {}", file);
        return false;
    }

    for (auto e = textureRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (strcmp(e->Value(), "texture") == 0)
        {
            std::string name = e->Attribute("name");
            ResourceInfo<sf::Texture> info;
            info.m_filePath = e->Attribute("filepath");
            m_resources.emplace(name, info);
        }
    }

    return true;
}