#include "Label.hpp"

namespace gui
{

Label::Label()
    : Element()
{
}

Label::~Label()
{
}

void Label::SetLabelInfo(const LabelInfo& info)
{
    m_info = info;

    m_shape.setScale(m_info.m_scale);

    if (m_info.m_texture)
    {
        m_shape.setTexture(m_info.m_texture.get(), true);
        m_shape.setTextureRect(m_info.m_rect);
        SetSize(sf::Vector2f(info.m_rect.size));
        Element::_UpdateText();
    }
    else
    {
        m_shape.setFillColor(info.m_color);
        m_shape.setOutlineColor(info.m_outlineColor);
        m_shape.setOutlineThickness(info.m_outlineSize);
    }
}

void Label::_RenderPrimitive(sf::RenderTarget* target)
{
    target->draw(m_shape);
}

void Label::_UpdatePosition()
{
    m_shape.setPosition(m_position);
}

void Label::_UpdateSize()
{
    m_shape.setSize(m_size);
}

} // namespace gui