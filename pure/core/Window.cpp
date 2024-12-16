#include "Window.hpp"

#include "EventManager.hpp"
#include "Log.hpp"
#include "SFML/Window/Event.hpp"
#include "scene/SceneManager.hpp"

#include <Windows.h>

Window::Window()
    : m_isTransparent(false),
      m_isOnTop(false),
      m_isClose(false),
      m_isFullscreen(false),
      m_isFocus(true),
      m_eventManager(nullptr)
{
}

Window::~Window() { UnInit(); }

void Window::Init(const std::string& title, const sf::Vector2u& size, uint32_t style, bool trans, bool onTop)
{
    m_title = title;
    m_size = size;
    m_isTransparent = trans;
    m_isOnTop = onTop;

    m_window.create(sf::VideoMode(size.x, size.y), title, m_isFullscreen == true ? sf::Style::Fullscreen : style);
    m_window.setKeyRepeatEnabled(false);

    if (m_isTransparent)
    {
        HWND hWnd = m_window.getSystemHandle();
        LONG wndStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
        wndStyle = wndStyle | WS_EX_LAYERED;
        SetWindowLong(hWnd, GWL_EXSTYLE, wndStyle);
        SetLayeredWindowAttributes(m_window.getSystemHandle(), RGB(128, 128, 128), 0, LWA_COLORKEY);
    }

    if (m_isOnTop)
    {
        HWND hwnd = m_window.getSystemHandle();
        SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }
}

void Window::UnInit() { m_window.close(); }

void Window::Update() { m_eventManager->Update(); }

void Window::HandleEvent()
{
    sf::Event event;
    while (m_window.pollEvent(event) == true)
    {
        switch (event.type)
        {
        case sf::Event::LostFocus:
        {
            DBG("Window::HandleEvent, LostFocus");
            m_isFocus = false;
            m_eventManager->SetFocus(false);
        }
        break;
        case sf::Event::GainedFocus:
        {
            DBG("Window::HandleEvent, GainedFocus");
            m_isFocus = true;
            m_eventManager->SetFocus(true);
        }
        break;
        case sf::Event::Closed:
        {
            m_isClose = true;
        }
        break;
        default:
        {
            m_eventManager->HandleEvent(event);
        }
        }
    }
}

void Window::BeginRender()
{
    if (m_isTransparent)
    {
        m_window.clear(sf::Color(128, 128, 128, 0));
    }
    else
    {
        m_window.clear(sf::Color::Black);
    }
}
void Window::SetPosition(const sf::Vector2i& position) { m_window.setPosition(position); }

void Window::SetView(const sf::View& view) { m_window.setView(view); }

void Window::SetEventManager(EventManager* manager)
{
    m_eventManager = manager;
    m_eventManager->AddEventCallback(SceneType::None, "Fullscreen_Toggle", &Window::_ToggleFullscreen, this);
    m_eventManager->AddEventCallback(SceneType::None, "Window_Close", &Window::_WindowClose, this);
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
    m_isFullscreen = !m_isFullscreen;
    UnInit();
    Init(m_title, m_size);
}

void Window::_WindowClose(EventDetail* detail) { m_isClose = true; }
