#include "Button.hpp"

#include "../common/Log.hpp"
#include "../pch.hpp" // IWYU pragma: keep
#include "Widget.hpp"

namespace gui
{

Button::Button()
    : Element()
{
}

Button::~Button()
{
}

bool Button::operator()(const sf::Event::MouseMoved& event)
{
    sf::Rect rect{m_position, m_size};
    if (rect.contains(sf::Vector2f{event.position}))
    {
        if (m_state == ElementState::Default)
        {
            m_state = ElementState::Hover;
            _UpdateCurrentState();
            m_signalQueue.emplace_back(Signal::OnHover, std::any());
            DBG("Button {} OnHover", m_name);
        }
        return true;
    }
    else
    {
        if (m_state == ElementState::Pressed || m_state == ElementState::Hover)
        {
            m_state = ElementState::Default;
            _UpdateCurrentState();
            m_signalQueue.emplace_back(Signal::OnLeave, std::any());
            DBG("Button {} OnDefault", m_name);
        }
    }

    return false;
}

bool Button::operator()(const sf::Event::MouseButtonPressed& event)
{
    if (m_state == ElementState::Hover)
    {
        m_state = ElementState::Pressed;
        _UpdateCurrentState();
        m_signalQueue.emplace_back(Signal::OnPress, std::any());
        return true;
    }

    return false;
}

bool Button::operator()(const sf::Event::MouseButtonReleased& event)
{
    if (m_state == ElementState::Pressed)
    {
        m_state = ElementState::Hover;
        _UpdateCurrentState();
        m_signalQueue.emplace_back(Signal::OnClicked, std::any());
        return true;
    }

    return false;
}

void Button::SetButtonStyle(ElementState state, ButtonStyle* style)
{
    m_styles[state] = style;
    _UpdateCurrentState();
}

ButtonStyle* Button::GetButtonStyle(ElementState state)
{
    if (m_styles.contains(state))
        return m_styles[state];
    return nullptr;
}

void Button::_RenderPrimitive(sf::RenderTarget* target)
{
    target->draw(m_shape);
}

void Button::_UpdatePosition()
{
    m_shape.setPosition(m_position);
}

void Button::_UpdateSize()
{
    m_shape.setSize(m_size);
}

void Button::_UpdateCurrentState()
{
    if (m_styles.contains(m_state))
    {
        auto& style = m_styles[m_state];

        m_shape.setScale(style->m_scale);

        if (style->m_texture)
        {
            m_shape.setTexture(style->m_texture.get());
            m_shape.setTextureRect(style->m_rect);
        }
        else
        {
            m_shape.setFillColor(style->m_color);
            m_shape.setOutlineColor(style->m_outlineColor);
            m_shape.setOutlineThickness(style->m_outlineSize);
        }

        if (!m_parent.expired())
        {
            m_parent.lock()->SetRedraw(true);
        }
    }
}

} // namespace gui