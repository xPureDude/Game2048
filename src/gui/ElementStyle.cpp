#include "ElementStyle.hpp" // IWYU pragma: keep

#include "../common/Log.hpp"
#include "../core/SharedContext.hpp"
#include "../pch.hpp" // IWYU pragma: keep
#include "../resource/FontManager.hpp"
#include "../resource/TextureManager.hpp"

template <typename T>
static void LoadTextureBase(T* style, const tinyxml2::XMLElement* elem)
{
    auto attr = elem->FindAttribute("texture");
    if (attr)
    {
        style->m_textureName = attr->Value();
    }

    attr = elem->FindAttribute("rectx");
    if (attr)
    {
        style->m_rect.position.x = attr->IntValue();
    }

    attr = elem->FindAttribute("recty");
    if (attr)
    {
        style->m_rect.position.y = attr->IntValue();
    }

    attr = elem->FindAttribute("rectw");
    if (attr)
    {
        style->m_rect.size.x = attr->IntValue();
    }

    attr = elem->FindAttribute("recth");
    if (attr)
    {
        style->m_rect.size.y = attr->IntValue();
    }
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

    LoadTextureBase(this, elem);

    m_texture = ctx->Get<TextureManager>()->RequestResource(m_textureName);
    if (!m_texture)
    {
        DBG("ButtonStyle::LoadFromXmlElement, texture is nullptr, textureName: {}", m_textureName);
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

    LoadTextureBase(this, elem);

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