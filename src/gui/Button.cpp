#include "Button.hpp"

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

void Button::SetButtonInfo(ElementState state, const ButtonInfo& info)
{
    m_infos[state] = info;
    _UpdateCurrentState();
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
    if (m_infos.contains(m_state))
    {
        auto& info = m_infos[m_state];

        m_shape.setScale(info.m_scale);

        if (info.m_texture)
        {
            m_shape.setTexture(info.m_texture.get());
            m_shape.setTextureRect(info.m_rect);
        }
        else
        {
            m_shape.setFillColor(info.m_color);
            m_shape.setOutlineColor(info.m_outlineColor);
            m_shape.setOutlineThickness(info.m_outlineSize);
        }
    }
}

} // namespace gui