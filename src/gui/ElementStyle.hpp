#ifndef ELEMENTSTYLE_HPP
#define ELEMENTSTYLE_HPP

#include "../tinyxml/tinyxml2.h"
#include "SFML/Graphics.hpp" // IWYU pragma: keep

#include <memory>
#include <string>

class SharedContext;

namespace gui
{

struct StyleSheet
{
    virtual ~StyleSheet() = default;
    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* elem, SharedContext* ctx) = 0;

    std::string m_name;
    sf::Vector2f m_scale{1, 1};
};

struct TextStyle : public StyleSheet
{
    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* elem, SharedContext* ctx) override;

    std::string m_fontName;
    std::shared_ptr<sf::Font> m_font;
    std::int32_t m_charSize{30};
    sf::Color m_color;
    std::uint32_t m_style;
};

struct ButtonStyle : public StyleSheet
{
    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* elem, SharedContext* ctx) override;

    // shape
    float m_outlineSize{0.f};
    sf::Color m_color;
    sf::Color m_outlineColor;

    // texture
    std::string m_textureName;
    std::shared_ptr<sf::Texture> m_texture;
    sf::IntRect m_rect;
};

struct LabelStyle : public StyleSheet
{
    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* elem, SharedContext* ctx) override;

    // shape
    float m_outlineSize{0.f};
    sf::Color m_color;
    sf::Color m_outlineColor;

    // texture
    std::string m_textureName;
    std::shared_ptr<sf::Texture> m_texture{nullptr};
    sf::IntRect m_rect;
};

struct WidgetStyle : public StyleSheet
{
    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* elem, SharedContext* ctx) override;

    sf::Color m_backColor{sf::Color::Transparent};
};

} // namespace gui

#endif // ELEMENTSTYLE_HPP