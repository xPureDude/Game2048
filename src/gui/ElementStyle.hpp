#ifndef ELEMENTSTYLE_HPP
#define ELEMENTSTYLE_HPP

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"

namespace gui
{

struct DefaultStyle
{
    std::uint32_t m_borderSize;
    sf::Color m_backColor;
    sf::RectangleShape m_back;
    sf::Color m_contentColor;
    sf::RectangleShape m_content;
    sf::Color m_controlColor;
    sf::RectangleShape m_control;
};

struct TextStyle
{
    TextStyle(const sf::Font& font)
        : m_text(font)
    {
    }

    float m_textSize;
    sf::Color m_textColor;
    sf::Text m_text;
};

struct TextureStyle
{
    TextureStyle(sf::Texture& texture)
        : m_sprite(texture)
    {
    }

    std::string m_textureName;
    sf::Sprite m_sprite;
};

struct ElementStyle
{
    std::shared_ptr<DefaultStyle> m_default;
    std::shared_ptr<TextStyle> m_textComp;
    std::shared_ptr<TextureStyle> m_texture;
};

} // namespace gui

#endif // ELEMENTSTYLE_HPP