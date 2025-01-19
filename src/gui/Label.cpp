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

void Label::SetLabelStyle(LabelStyle* style)
{
    m_style = style;

    m_shape.setScale(m_style->m_scale);

    if (m_style->m_texture)
    {
        m_shape.setTexture(m_style->m_texture.get(), true);
        m_shape.setTextureRect(m_style->m_rect);
    }
    else
    {
        m_shape.setFillColor(m_style->m_color);
        m_shape.setOutlineColor(m_style->m_outlineColor);
        m_shape.setOutlineThickness(m_style->m_outlineSize);
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