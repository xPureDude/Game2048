#include "PureFramework.hpp"

#include "../gui/GuiManager.hpp"
#include "../input/InputManager.hpp"
#include "../resource/FontManager.hpp"
#include "../resource/TextureManager.hpp"
#include "../scene/SceneManager.hpp"
#include "ConfigManager.hpp"
#include "Window.hpp"

#define FRAME_ELASPED 1000.0f / 144.0f
const sf::Time fpsNil = sf::Time::Zero;
const sf::Time fps60 = sf::milliseconds(1000 / 60.f);
const sf::Time fps144 = sf::milliseconds(1000 / 60.f);

PureFramework::PureFramework()
{
    m_fpsTime = fps144;
}

PureFramework::~PureFramework()
{
}

bool PureFramework::Init()
{
    m_ctx.Emplace<ConfigManager>([](void* obj) { delete (ConfigManager*)obj; });
    m_ctx.Emplace<TextureManager>([](void* obj) { delete (TextureManager*)obj; });
    m_ctx.Emplace<FontManager>([](void* obj) { delete (FontManager*)obj; });
    m_ctx.Emplace<InputManager>([](void* obj) { delete (InputManager*)obj; });
    m_ctx.Emplace<Window>([](void* obj) { delete (Window*)obj; }, &m_ctx);
    m_ctx.Emplace<GuiManager>([](void* obj) { delete (GuiManager*)obj; }, &m_ctx);
    m_ctx.Emplace<SceneManager>([](void* obj) { delete (SceneManager*)obj; }, &m_ctx);

    return true;
}

void PureFramework::UnInit()
{
    m_ctx.Release<SceneManager>();
    m_ctx.Release<GuiManager>();
    m_ctx.Release<TextureManager>();
    m_ctx.Release<FontManager>();
    m_ctx.Release<Window>();
    m_ctx.Release<InputManager>();
    m_ctx.Release<ConfigManager>();
}

void PureFramework::Run()
{
    m_clock.restart();
    while (m_ctx.Get<Window>()->IsClose() == false)
    {
        m_elasped = m_clock.getElapsedTime();
        if (m_elasped < fps144)
        {
            sf::sleep(fps144 - m_elasped);
        }
        m_elasped = m_clock.restart();

        HandleEvent();
        Update();
        Render();
        LateUpdate();

        _SlowUpdate();
    }
}

void PureFramework::Update()
{
    m_ctx.Get<InputManager>()->Update(m_elasped);
    m_ctx.Get<GuiManager>()->Update(m_elasped);
    m_ctx.Get<SceneManager>()->Update(m_elasped);
}

void PureFramework::Render()
{
    m_ctx.Get<Window>()->BeginRender();
    m_ctx.Get<SceneManager>()->Render();
    m_ctx.Get<GuiManager>()->Render();
    m_ctx.Get<Window>()->EndRender();
}

void PureFramework::HandleEvent()
{
    Window* window = m_ctx.Get<Window>();
    while (const std::optional event = window->PollEvent())
    {
        m_ctx.Get<InputManager>()->HandleInput(event.value());
        m_ctx.Get<GuiManager>()->HandleInput(event.value());
    }
}

void PureFramework::LateUpdate()
{
    m_ctx.Get<SceneManager>()->ProcessRemoves();
}

void PureFramework::_SlowUpdate()
{
    m_lastSlowUpdate += m_elasped;
    if (m_lastSlowUpdate < sf::seconds(1.0))
        return;

    m_lastSlowUpdate -= sf::seconds(1.0);
    m_ctx.Get<TextureManager>()->SlowUpdate();
    m_ctx.Get<FontManager>()->SlowUpdate();
}