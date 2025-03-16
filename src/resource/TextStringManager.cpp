#include "TextStringManager.hpp"

#include "../pch.hpp" // IWYU pragma: keep

TextStringManager::TextStringManager()
{
}

TextStringManager::~TextStringManager()
{
}

bool TextStringManager::LoadTextStringFromFile(const std::string_view& file, const std::string& local)
{
    tinyxml2::XMLDocument xmlDoc;
    if (xmlDoc.LoadFile(file.data()) != tinyxml2::XML_SUCCESS)
    {
        ERR("TextStringManager::LoadTextStringFromFile, failed to load file: {}", file);
        return false;
    }

    tinyxml2::XMLElement* root = xmlDoc.RootElement();
    if (!root)
    {
        ERR("TextStringManager::LoadTextStringFromFile, RootElement is nullptr, file: {}", file);
        return false;
    }

    for (auto text = root->FirstChildElement(); text != nullptr; text = text->NextSiblingElement())
    {
        if (strcmp(text->Value(), "Text") == 0)
        {
            TextString id = static_cast<TextString>(text->Unsigned64Attribute("id"));
            std::string str;
            if (auto e = text->FirstChildElement(local.c_str()); e)
            {
                str = e->Attribute("string");
            }
            m_textStrings[id] = sf::String::fromUtf8(str.begin(), str.end());
        }
    }
    return true;
}

sf::String TextStringManager::FindTextString(TextString id)
{
    if (m_textStrings.contains(id))
        return m_textStrings[id];

    DBG("TextStringManager::FindTextString, id not exists: {}", static_cast<std::uint64_t>(id));
    return {};
}