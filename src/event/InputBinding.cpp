#include "InputBinding.hpp"

#include "EventManager.hpp"
#include "GlobalEvent.hpp"
#include "InputEvent.hpp"
#include "core/SharedContext.hpp"

namespace input
{

Binding::Binding()
    : m_isTriggered(false)
{
}

Binding::~Binding()
{
}

void Binding::TriggerEvent()
{
    SetTriggered(false);
    _DoTriggerEvent();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool WindowClose::operator()(const sf::Event::Closed& event)
{
    m_isTriggered = true;
    return true;
}

void WindowClose::_DoTriggerEvent()
{
    SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::WindowClose>();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool FullscreenToggle::operator()(const sf::Event::KeyPressed& event)
{
    if (event.alt && event.scancode == sf::Keyboard::Scancode::Enter)
    {
        m_isTriggered = true;
        return true;
    }
    return false;
}

void FullscreenToggle::_DoTriggerEvent()
{
    SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::FullscreenToggle>();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool MouseMove::operator()(const sf::Event::MouseMoved& event)
{
    m_prevPos = m_curPos;
    m_curPos = {static_cast<float>(event.position.x), static_cast<float>(event.position.y)};
    m_isTriggered = true;

    return false;
}

void MouseMove::_DoTriggerEvent()
{
    auto event = SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::MouseMove>();
    event->m_curPos = m_curPos;
    event->m_prevPos = m_prevPos;
    event->m_deltaPos = m_curPos - m_prevPos;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool MouseButtonDown::operator()(const sf::Event::MouseButtonPressed& event)
{
    m_button = event.button;
    m_isTriggered = true;

    return true;
}

void MouseButtonDown::_DoTriggerEvent()
{
    auto event = SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::MouseButtonDown>();
    event->m_button = m_button;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool MouseButtonUp::operator()(const sf::Event::MouseButtonReleased& event)
{
    m_button = event.button;
    m_isTriggered = true;

    return true;
}

void MouseButtonUp::_DoTriggerEvent()
{
    auto event = SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::MouseButtonUp>();
    event->m_button = m_button;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool MoveLeft::operator()(const sf::Event::KeyPressed& event)
{
    if (event.scancode == sf::Keyboard::Scancode::Left || event.scancode == sf::Keyboard::Scancode::A)
    {
        m_isTriggered = true;
        return true;
    }
    return false;
}

void MoveLeft::_DoTriggerEvent()
{
    SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::MoveLeft>();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool MoveRight::operator()(const sf::Event::KeyPressed& event)
{
    if (event.scancode == sf::Keyboard::Scancode::Right || event.scancode == sf::Keyboard::Scancode::D)
    {
        m_isTriggered = true;
        return true;
    }
    return false;
}

void MoveRight::_DoTriggerEvent()
{
    SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::MoveRight>();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool MoveUp::operator()(const sf::Event::KeyPressed& event)
{
    if (event.scancode == sf::Keyboard::Scancode::Up || event.scancode == sf::Keyboard::Scancode::W)
    {
        m_isTriggered = true;
        return true;
    }
    return false;
}

void MoveUp::_DoTriggerEvent()
{
    SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::MoveUp>();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool MoveDown::operator()(const sf::Event::KeyPressed& event)
{
    if (event.scancode == sf::Keyboard::Scancode::Down || event.scancode == sf::Keyboard::Scancode::S)
    {
        m_isTriggered = true;
        return true;
    }
    return false;
}

void MoveDown::_DoTriggerEvent()
{
    SharedContext::Instance()->Get<EventManager>()->PushEvent<evt::MoveDown>();
}

} // namespace input