#include "Window.hpp"

#include <Windows.h>

#include "ConfigManager.hpp"
#include "SharedContext.hpp"
#include "common/Utility.hpp"
#include "input/InputManager.hpp"


Window::Window()
    : m_isClose(false),
      m_isFocus(true),
      m_state(sf::State::Windowed)
{
    auto inputManager = SharedContext::Instance()->Get<InputManager>();
    inputManager->AddInputBindingCallback(SceneType::None, ib::BindType::FullscreenToggle, "Window_FullscreenToggle", BindCallback(&Window::_ToggleFullscreen));
    inputManager->AddInputBindingCallback(SceneType::None, ib::BindType::WindowClose, "Window_WindowClose", BindCallback(&Window::_WindowClose));

    auto& windowConfig = SharedContext::Instance()->Get<ConfigManager>()->GetWindowConfig();
    m_title = windowConfig.m_title;
    m_size = {windowConfig.m_width, windowConfig.m_height};
    m_style = windowConfig.m_style;
    m_state = static_cast<sf::State>(windowConfig.m_state);

    m_window.create(sf::VideoMode(m_size), m_title, m_style, m_state);
    m_window.setKeyRepeatEnabled(false);
}

Window::~Window()
{
    auto inputManager = SharedContext::Instance()->Get<InputManager>();
    inputManager->DelInputBindingCallback(SceneType::None, ib::BindType::FullscreenToggle, "Window_FullscreenToggle");
    inputManager->DelInputBindingCallback(SceneType::None, ib::BindType::WindowClose, "Window_WindowClose");
}

std::optional<sf::Event> Window::PollEvent()
{
    return m_window.pollEvent();
}

void Window::BeginRender()
{
    m_window.clear(sf::Color::Black);
}

void Window::Render(sf::Drawable& drawable)
{
    auto ret = m_window.setActive();
    m_window.draw(drawable);
}

void Window::EndRender()
{
    m_window.display();
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

void Window::_ToggleFullscreen(const std::any& param)
{
    if (m_state == sf::State::Windowed)
        m_state = sf::State::Fullscreen;
    else
        m_state = sf::State::Windowed;

    m_window.close();
    m_window.create(sf::VideoMode(m_size), m_title, m_style, m_state);
}

void Window::_WindowClose(const std::any& param)
{
    m_isClose = true;
}
