#pragma once

#include <optional>
#include <string>

#include "SFML/Graphics.hpp" // IWYU pragma: keep
#include "SFML/Window.hpp"   // IWYU pragma: keep
#include "event/ScopedEventListener.hpp"

class Window
{
public:
    Window();
    ~Window();

    std::optional<sf::Event> PollEvent();
    void BeginRender();
    void Render(sf::Drawable& drawable);
    void EndRender();

    void Close() { m_isClose = true; }

    bool IsClose() { return m_isClose; }
    bool IsFullscreen() { return m_state == sf::State::Fullscreen; }
    bool IsWindowed() { return m_state == sf::State::Windowed; }
    bool IsFocus() { return m_isFocus; }

    void SetSize(const sf::Vector2u& size);
    void SetPosition(const sf::Vector2i& position);
    void SetView(const sf::View& view);

    void MoveBy(const sf::Vector2i& velocity);

    sf::Vector2i GetPosition() { return m_window.getPosition(); }
    std::string GetTitle() { return m_title; }
    sf::Vector2u GetSize() { return m_size; }
    sf::RenderWindow& GetRenderWindow() { return m_window; }
    sf::View GetDefaultView() { return m_window.getDefaultView(); }

    sf::FloatRect GetViewSpace();

private:
    void _OnToggleFullscreen(evt::Base* e);
    void _OnWindowClose(evt::Base* e);

private:
    bool m_isClose;
    bool m_isFocus;
    sf::Vector2u m_size;
    std::string m_title;

    std::int32_t m_style;
    sf::State m_state;
    sf::RenderWindow m_window;

    ScopedEventListener m_eventListener;
};