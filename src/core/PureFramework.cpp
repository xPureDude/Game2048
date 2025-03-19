#include "PureFramework.hpp"

#include "../gui/GuiManager.hpp"
#include "../input/InputManager.hpp"
#include "../resource/FontManager.hpp"
#include "../resource/TextStringManager.hpp"
#include "../resource/TextureManager.hpp"
#include "../scene/SceneManager.hpp"
#include "ConfigManager.hpp"
#include "SharedContext.hpp"
#include "Window.hpp"

#define FRAME_ELASPED 1000.0f / 144.0f
const sf::Time fpsNil = sf::Time::Zero;
const sf::Time fps60 = sf::milliseconds(1000 / 60.f);
const sf::Time fps144 = sf::milliseconds(1000 / 144.f);

PureFramework::PureFramework()
{
    m_fpsTime = fps144;
}

PureFramework::~PureFramework()
{
}

bool PureFramework::Init()
{
    SharedContext::Instance().Emplace<ConfigManager>([](void* obj) { delete (ConfigManager*)obj; });
    SharedContext::Instance().Emplace<TextStringManager>([](void* obj) { delete (TextStringManager*)obj; });
    SharedContext::Instance().Emplace<TextureManager>([](void* obj) { delete (TextureManager*)obj; });
    SharedContext::Instance().Emplace<FontManager>([](void* obj) { delete (FontManager*)obj; });
    SharedContext::Instance().Emplace<InputManager>([](void* obj) { delete (InputManager*)obj; });
    SharedContext::Instance().Emplace<Window>([](void* obj) { delete (Window*)obj; });
    SharedContext::Instance().Emplace<GuiManager>([](void* obj) { delete (GuiManager*)obj; });
    SharedContext::Instance().Emplace<SceneManager>([](void* obj) { delete (SceneManager*)obj; });

    return true;
}

void PureFramework::UnInit()
{
    SharedContext::Instance().Release<SceneManager>();
    SharedContext::Instance().Release<GuiManager>();
    SharedContext::Instance().Release<TextureManager>();
    SharedContext::Instance().Release<FontManager>();
    SharedContext::Instance().Release<Window>();
    SharedContext::Instance().Release<InputManager>();
    SharedContext::Instance().Release<TextStringManager>();
    SharedContext::Instance().Release<ConfigManager>();
}

void PureFramework::Run()
{
    m_clock.restart();
    auto window = SharedContext::Instance().Get<Window>();
    while (window->IsClose() == false)
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
    SharedContext::Instance().Get<InputManager>()->Update(m_elasped);
    SharedContext::Instance().Get<GuiManager>()->Update(m_elasped);
    SharedContext::Instance().Get<SceneManager>()->Update(m_elasped);
}

void PureFramework::Render()
{
    SharedContext::Instance().Get<Window>()->BeginRender();
    SharedContext::Instance().Get<SceneManager>()->Render();
    SharedContext::Instance().Get<GuiManager>()->Render();
    SharedContext::Instance().Get<Window>()->EndRender();
}

void PureFramework::HandleEvent()
{
    Window* window = SharedContext::Instance().Get<Window>();
    while (const std::optional event = window->PollEvent())
    {
        SharedContext::Instance().Get<InputManager>()->HandleInput(event.value());
        SharedContext::Instance().Get<GuiManager>()->HandleInput(event.value());
    }
}

void PureFramework::LateUpdate()
{
    SharedContext::Instance().Get<SceneManager>()->ProcessRemoves();
}

void PureFramework::_SlowUpdate()
{
    m_lastSlowUpdate += m_elasped;
    if (m_lastSlowUpdate < sf::seconds(1.0))
        return;

    m_lastSlowUpdate -= sf::seconds(1.0);
    SharedContext::Instance().Get<TextureManager>()->SlowUpdate();
    SharedContext::Instance().Get<FontManager>()->SlowUpdate();
}