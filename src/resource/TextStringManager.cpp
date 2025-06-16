#include "TextStringManager.hpp"

#include <format>

#include "tinyxml/tinyxml2.h"

#include "common/Log.hpp"
#include "core/ConfigManager.hpp"
#include "core/SharedContext.hpp"
#include "event/GlobalEvent.hpp"

TextStringManager::TextStringManager()
{
    SubscribeEvent<evt::LanguageChanged>(BindCallback(&TextStringManager::_OnLanguageChanged));
}

TextStringManager::~TextStringManager()
{
}

bool TextStringManager::LoadTextStringFromFile(const std::string_view& file, const std::string& local)
{
    m_filePath = file;

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
            std::string id = text->Attribute("id");
            std::string str;
            if (auto e = text->FirstChildElement(local.c_str()); e)
            {
                str = e->GetText();
            }
            m_textStrings[id] = sf::String::fromUtf8(str.begin(), str.end());
        }
    }
    return true;
}

sf::String TextStringManager::FindTextString(const std::string& id)
{
    if (m_textStrings.contains(id))
        return m_textStrings[id];

    DBG("TextStringManager::FindTextString, id not exists: {}", id);
    return {};
}

sf::String TextStringManager::PackFormatString(const sf::String& str, const std::vector<std::string>& args)
{
    auto tempString = str.toUtf8();
    std::string u8String(tempString.begin(), tempString.end());
    std::string result;
    switch (args.size())
    {
    case 0:
        result = u8String;
        break;
    case 1:
        result = std::vformat(std::string_view(u8String), std::make_format_args(args[0]));
        break;
    case 2:
        result = std::vformat(std::string_view(u8String), std::make_format_args(args[0], args[1]));
        break;
    case 3:
        result = std::vformat(std::string_view(u8String), std::make_format_args(args[0], args[1], args[2]));
        break;
    case 4:
        result = std::vformat(std::string_view(u8String), std::make_format_args(args[0], args[1], args[2], args[3]));
        break;
    default:
        break;
    }
    return sf::String::fromUtf8(result.begin(), result.end());
}

void TextStringManager::_OnLanguageChanged(evt::Base* event)
{
    auto newLanguage = SharedContext::Instance()->Get<ConfigManager>()->GetVisualConfig().m_language;
    LoadTextStringFromFile(m_filePath, newLanguage);
}