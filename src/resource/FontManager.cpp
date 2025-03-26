#include "FontManager.hpp"

#include "common/Log.hpp"

bool FontManager::LoadResourceInfoFromFile(const std::string& file)
{
    if (file.ends_with(".xml"))
    {
        return _LoadResourceInfoFromXml(file);
    }
    return false;
}

bool FontManager::_LoadResourceInfoFromXml(const std::string& file)
{
    tinyxml2::XMLDocument xmlDoc;
    if (xmlDoc.LoadFile(file.c_str()) != tinyxml2::XML_SUCCESS)
    {
        ERR("FontManager::_LoadResourceInfoFromXml, failed to load file: {}", file);
        return false;
    }

    tinyxml2::XMLElement* fontRoot = xmlDoc.RootElement();
    if (!fontRoot)
    {
        ERR("FontManager::_LoadResourceInfoFromXml, RootElement is nullptr, file: {}", file);
        return false;
    }

    for (auto e = fontRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (strcmp(e->Value(), "font") == 0)
        {
            std::string name = e->Attribute("name");
            ResourceInfo<sf::Font> info;
            info.m_filePath = e->Attribute("filepath");
            m_resources.emplace(name, info);
        }
    }

    return true;
}