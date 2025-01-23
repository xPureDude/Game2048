#include "ElementStyle.hpp" // IWYU pragma: keep

#include "../common/Log.hpp"
#include "../core/SharedContext.hpp"
#include "../pch.hpp" // IWYU pragma: keep
#include "../resource/FontManager.hpp"
#include "../resource/TextureManager.hpp"

template <typename T>
static bool LoadTextureBase(T* style, const tinyxml2::XMLElement* elem, SharedContext* ctx)
{
    auto attr = elem->FindAttribute("texture");
    if (!attr)
    {
        DBG("LoadTextureBase, attr 'texture' not found");
        return false;
    }

    style->m_textureName = attr->Value();

    style->m_texture = ctx->Get<TextureManager>()->RequestResource(style->m_textureName);
    if (!style->m_texture)
    {
        DBG("LoadTextureBase, failed to Texture RequestResource: {}", style->m_textureName);
        return false;
    }

    if ((elem->QueryIntAttribute("rectx", &style->m_rect.position.x) != tinyxml2::XML_SUCCESS)
        || (elem->QueryIntAttribute("recty", &style->m_rect.position.y) != tinyxml2::XML_SUCCESS)
        || (elem->QueryIntAttribute("rectw", &style->m_rect.size.x) != tinyxml2::XML_SUCCESS)
        || (elem->QueryIntAttribute("recth", &style->m_rect.size.y) != tinyxml2::XML_SUCCESS))
        return false;

    return true;
}

namespace gui
{

bool TextStyle::LoadFromXmlElement(tinyxml2::XMLElement* elem, SharedContext* ctx)
{
    auto attr = elem->FindAttribute("name");
    if (!attr)
    {
        DBG("TextStyle::LoadFromXmlElement, attribute 'name' not found");
        return false;
    }
    m_name = attr->Value();

    attr = elem->FindAttribute("font");
    if (!attr)
    {
        DBG("TextStyle::LoadFromXmlElement, attribute 'font' not found");
        return false;
    }
    m_fontName = attr->Value();
    m_font = ctx->Get<FontManager>()->RequestResource(m_fontName);
    if (!m_font)
    {
        DBG("TextStyle::LoadFromXmlElement, font is nullptr, fontName: {}", m_fontName);
        return false;
    }

    attr = elem->FindAttribute("charsize");
    if (attr)
    {
        m_charSize = attr->IntValue();
    }

    attr = elem->FindAttribute("color");
    if (attr)
    {
        m_color = sf::Color(attr->IntValue());
    }

    attr = elem->FindAttribute("style");
    if (attr)
    {
        if (std::strcmp("Bold", attr->Value()))
        {
            m_style = sf::Text::Style::Bold;
        }
        else if (std::strcmp("Italic", attr->Value()))
        {
            m_style = sf::Text::Style::Italic;
        }
        else if (std::strcmp("StrikeThrough", attr->Value()))
        {
            m_style = sf::Text::Style::StrikeThrough;
        }
        else if (std::strcmp("Underlined", attr->Value()))
        {
            m_style = sf::Text::Style::Underlined;
        }
    }

    return true;
}

bool ButtonStyle::LoadFromXmlElement(tinyxml2::XMLElement* elem, SharedContext* ctx)
{
    auto attr = elem->FindAttribute("name");
    if (!attr)
    {
        DBG("ButtonStyle::LoadFromXmlElement, attribute 'name' not found");
        return false;
    }
    m_name = attr->Value();

    if (!LoadTextureBase(this, elem, ctx))
    {
        DBG("ButtonStyle::LoadFromXmlElement, LoadTextureBase failed");
        return false;
    }

    return true;
}

bool LabelStyle::LoadFromXmlElement(tinyxml2::XMLElement* elem, SharedContext* ctx)
{
    auto attr = elem->FindAttribute("name");
    if (!attr)
    {
        DBG("LabelStyle::LoadFromXmlElement, attribute 'name' not found");
        return false;
    }
    m_name = attr->Value();

    if (!LoadTextureBase(this, elem, ctx))
    {
        DBG("LabelStyle::LoadFromXmlElement, LoadTextureBase failed");
        return false;
    }

    return true;
}

bool WidgetStyle::LoadFromXmlElement(tinyxml2::XMLElement* elem, SharedContext* ctx)
{
    auto attr = elem->FindAttribute("name");
    if (!attr)
    {
        DBG("WidgetStyle::LoadFromXmlElement, attribute 'name' not found");
        return false;
    }
    m_name = attr->Value();

    attr = elem->FindAttribute("color");
    if (!attr)
    {
        DBG("WidgetStyle::LoadFromXmlElement, WidgetStyle attribute 'color' not found");
        return false;
    }
    m_backColor = sf::Color(attr->IntValue());

    return true;
}

} // namespace gui