#include "InputBinding.hpp"

namespace ib
{

Binding::Binding(BindType bind)
    : m_bind(bind),
      m_isTriggered(false)
{
}

Binding::~Binding()
{
}

void Binding::AddBindingCallback(const std::string_view& name, CallbackType callback)
{
    assert(!m_callbacks.contains(name));
    m_callbacks.emplace(name, callback);
}

void Binding::DelBindingCallback(const std::string_view& name)
{
    assert(m_callbacks.contains(name));
    m_callbacks.erase(name);
}

void Binding::TriggerCallbacks()
{
    for (auto& callbackIter : m_callbacks)
    {
        callbackIter.second(m_param);
    }
    m_isTriggered = false;
}

WindowClose::WindowClose()
    : Binding(BindType::WindowClose)
{
}

bool WindowClose::operator()(const sf::Event::Closed& event)
{
    m_isTriggered = true;
    return true;
}

FullscreenToggle::FullscreenToggle()
    : Binding(BindType::FullscreenToggle)
{
}

bool FullscreenToggle::operator()(const sf::Event::KeyPressed& event)
{
    if (event.alt && event.scancode == sf::Keyboard::Scancode::Enter)
    {
        m_isTriggered = true;
    }
    return true;
}

MoveLeft::MoveLeft()
    : Binding(BindType::MoveLeft)
{
}

bool MoveLeft::operator()(const sf::Event::KeyPressed& event)
{
    if (event.scancode == sf::Keyboard::Scancode::Left || event.scancode == sf::Keyboard::Scancode::A)
    {
        m_isTriggered = true;
    }
    return true;
}

MoveRight::MoveRight()
    : Binding(BindType::MoveRight)
{
}

bool MoveRight::operator()(const sf::Event::KeyPressed& event)
{
    if (event.scancode == sf::Keyboard::Scancode::Right || event.scancode == sf::Keyboard::Scancode::D)
    {
        m_isTriggered = true;
    }
    return true;
}

MoveUp::MoveUp()
    : Binding(BindType::MoveUp)
{
}

bool MoveUp::operator()(const sf::Event::KeyPressed& event)
{
    if (event.scancode == sf::Keyboard::Scancode::Up || event.scancode == sf::Keyboard::Scancode::W)
    {
        m_isTriggered = true;
    }
    return true;
}

MoveDown::MoveDown()
    : Binding(BindType::MoveDown)
{
}

bool MoveDown::operator()(const sf::Event::KeyPressed& event)
{
    if (event.scancode == sf::Keyboard::Scancode::Down || event.scancode == sf::Keyboard::Scancode::S)
    {
        m_isTriggered = true;
    }
    return true;
}

} // namespace ib

ib::BindingPtr InputBindingFactory::CreateInputBinding(ib::BindType type)
{
    using namespace ib;
    switch (type)
    {
    case BindType::WindowClose:
        return std::make_shared<WindowClose>();

    case BindType::FullscreenToggle:
        return std::make_shared<FullscreenToggle>();

    case BindType::MoveLeft:
        return std::make_shared<MoveLeft>();

    case BindType::MoveRight:
        return std::make_shared<MoveRight>();

    case BindType::MoveUp:
        return std::make_shared<MoveUp>();

    case BindType::MoveDown:
        return std::make_shared<MoveDown>();

    default:
        return nullptr;
    }
}