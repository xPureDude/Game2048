#include "Window.hpp"

#include "../event/EventManager.hpp"

#include <Windows.h>

Window::Window(EventManager* eventManager)
    : m_isClose(false),
      m_isFocus(true),
      m_state(sf::State::Windowed),
      m_eventManager(eventManager)
{
    m_eventManager->AddInputBindingCallback<Window>(SceneType::None, ib::BindType::FullscreenToggle, "Window_FullscreenToggle", &Window::_ToggleFullscreen, this);
    m_eventManager->AddInputBindingCallback<Window>(SceneType::None, ib::BindType::WindowClose, "Window_WindowClose", &Window::_WindowClose, this);
}

Window::~Window()
{
    m_eventManager->DelInputBindingCallback(SceneType::None, ib::BindType::FullscreenToggle, "Window_FullscreenToggle");
    m_eventManager->DelInputBindingCallback(SceneType::None, ib::BindType::WindowClose, "Window_WindowClose");

    UnInit();
}

void Window::Init(const std::string& title, const sf::Vector2u& size, std::int32_t style, sf::State state)
{
    m_title = title;
    m_size = size;
    m_style = style;
    m_state = state;

    m_window.create(sf::VideoMode({size.x, size.y}), title, m_style, m_state);
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
        m_eventManager->HandleEvent(event.value());
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

void Window::_ToggleFullscreen()
{
    if (m_state == sf::State::Windowed)
        m_state = sf::State::Fullscreen;
    else
        m_state = sf::State::Windowed;
    UnInit();
    Init(m_title, m_size, m_style, m_state);
}

void Window::_WindowClose()
{
    m_isClose = true;
}
