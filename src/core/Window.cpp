#include "Window.hpp"

#include "../event/EventManager.hpp"

#include <Windows.h>

Window::Window(EventManager* eventManager)
    : m_isClose(false),
      m_isFocus(true),
      m_state(sf::State::Windowed),
      m_eventManager(eventManager)
{
    m_eventManager->AddEventCallback(SceneType::None, "Fullscreen_Toggle", &Window::_ToggleFullscreen, this);
    m_eventManager->AddEventCallback(SceneType::None, "Window_Close", &Window::_WindowClose, this);
}

Window::~Window()
{
    UnInit();
}

void Window::Init(const std::string& title, const sf::Vector2u& size, sf::State state)
{
    m_title = title;
    m_size = size;
    m_state = state;

    m_window.create(sf::VideoMode({size.x, size.y}), title, m_state);
    m_window.setKeyRepeatEnabled(false);
}

void Window::UnInit()
{
    m_window.close();
}

void Window::Update()
{
    m_eventManager->Update();
}

void Window::HandleEvent()
{
    while (const std::optional<sf::Event> event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            m_isClose = true;
        }
        else if (event->is<sf::Event::FocusLost>())
        {
            m_isFocus = false;
            m_eventManager->SetFocus(false);
        }
        else if (event->is<sf::Event::FocusGained>())
        {
            m_isFocus = true;
            m_eventManager->SetFocus(true);
        }
        else
            m_eventManager->HandleEvent(event);
    }
}

void Window::BeginRender()
{
    m_window.clear(sf::Color::Black);
}

void Window::SetPosition(const sf::Vector2i& position)
{
    m_window.setPosition(position);
}

void Window::SetView(const sf::View& view)
{
    m_window.setView(view);
}

void Window::MoveBy(const sf::Vector2i& velocity)
{
    auto position = m_window.getPosition();
    m_window.setPosition(position + velocity);
}

sf::FloatRect Window::GetViewSpace()
{
    sf::Vector2f center = m_window.getView().getCenter();
    sf::Vector2f size = m_window.getView().getSize();
    sf::Vector2f halfSize(size.x / 2.0f, size.y / 2.0f);
    return sf::FloatRect(center - halfSize, size);
}

void Window::_ToggleFullscreen(EventDetail* detail)
{
    if (m_state == sf::State::Windowed)
        m_state = sf::State::Fullscreen;
    else
        m_state = sf::State::Windowed;
    UnInit();
    Init(m_title, m_size, m_state);
}

void Window::_WindowClose(EventDetail* detail)
{
    m_isClose = true;
}
