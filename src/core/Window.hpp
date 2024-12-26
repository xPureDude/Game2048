#pragma once

#include "../pch.hpp" // IWYU pragma: keep

class Window
{
public:
    Window(SharedContext* ctx);
    ~Window();

    void Init(const std::string& title, const sf::Vector2u& size, std::int32_t style, sf::State state);
    void UnInit();

    std::optional<sf::Event> PollEvent();
    void BeginRender();
    void Render(sf::Drawable& drawable) { m_window.draw(drawable); }
    void EndRender() { m_window.display(); }

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
    sf::RenderWindow* GetRenderWindow() { return &m_window; }
    sf::View GetDefaultView() { return m_window.getDefaultView(); }

    sf::FloatRect GetViewSpace();

private:
    void _ToggleFullscreen();
    void _WindowClose();

private:
    SharedContext* m_ctx;

    bool m_isClose;
    bool m_isFocus;
    sf::Vector2u m_size;
    std::string m_title;

    std::int32_t m_style;
    sf::State m_state;
    sf::RenderWindow m_window;
};
