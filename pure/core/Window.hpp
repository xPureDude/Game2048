#pragma once

#include "SFML/System/Vector2.hpp"

#include <SFML/Graphics.hpp>
#include <string>

class EventDetail;
class EventManager;

class Window
{
public:
    Window();
    ~Window();

    void Init(const std::string& title, const sf::Vector2u& size, uint32_t style = sf::Style::Default, bool trans = false, bool onTop = false);
    void UnInit();

    void Update();
    void HandleEvent();
    void BeginRender();
    void Render(sf::Drawable& drawable) { m_window.draw(drawable); }
    void EndRender() { m_window.display(); }

    void Close() { m_isClose = true; }

    bool IsClose() { return m_isClose; }
    bool IsFullscreen() { return m_isFullscreen; }
    bool IsFocus() { return m_isFocus; }

    void SetSize(const sf::Vector2u& size);
    void SetPosition(const sf::Vector2i& position);
    void SetView(const sf::View& view);
    void SetEventManager(EventManager* manager);

    void MoveBy(const sf::Vector2i& velocity);

    sf::Vector2i GetPosition() { return m_window.getPosition(); }
    std::string GetTitle() { return m_title; }
    sf::Vector2u GetSize() { return m_size; }
    sf::RenderWindow* GetRenderWindow() { return &m_window; }
    sf::View GetDefaultView() { return m_window.getDefaultView(); }

    sf::FloatRect GetViewSpace();

private:
    void _ToggleFullscreen(EventDetail* detail);
    void _WindowClose(EventDetail* detail);

private:
    bool m_isTransparent;
    bool m_isOnTop;
    bool m_isClose;
    bool m_isFullscreen;
    bool m_isFocus;
    std::string m_title;
    sf::Vector2u m_size;
    sf::RenderWindow m_window;
    EventManager* m_eventManager;
};
