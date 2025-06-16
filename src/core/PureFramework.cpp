#include "PureFramework.hpp"

#include "ConfigManager.hpp"
#include "SharedContext.hpp"
#include "Window.hpp"
#include "common/Random.hpp"
#include "event/EventManager.hpp"
#include "event/InputManager.hpp"
#include "gui/GuiManager.hpp"
#include "resource/FontManager.hpp"
#include "resource/TextStringManager.hpp"
#include "resource/TextureManager.hpp"
#include "scene/SceneManager.hpp"

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
    SharedContext::Instance()->Emplace<ConfigManager>();
    SharedContext::Instance()->Emplace<Random>();
    SharedContext::Instance()->Emplace<InputManager>();
    SharedContext::Instance()->Emplace<EventManager>();
    SharedContext::Instance()->Emplace<TextStringManager>();
    SharedContext::Instance()->Emplace<TextureManager>();
    SharedContext::Instance()->Emplace<FontManager>();
    SharedContext::Instance()->Emplace<Window>();
    SharedContext::Instance()->Emplace<GuiManager>();
    SharedContext::Instance()->Emplace<SceneManager>();

    return true;
}

void PureFramework::UnInit()
{
    SharedContext::Instance()->ReleaseAll();
}

void PureFramework::Run()
{
    m_clock.restart();
    auto window = SharedContext::Instance()->Get<Window>();
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
    SharedContext::Instance()->Get<GuiManager>()->UpdateFrame(m_elasped);
    SharedContext::Instance()->Get<SceneManager>()->UpdateFrame(m_elasped);
}

void PureFramework::Render()
{
    SharedContext::Instance()->Get<Window>()->BeginRender();
    SharedContext::Instance()->Get<SceneManager>()->Render();
    SharedContext::Instance()->Get<GuiManager>()->Render();
    SharedContext::Instance()->Get<Window>()->EndRender();
}

void PureFramework::HandleEvent()
{
    Window* window = SharedContext::Instance()->Get<Window>();
    InputManager* inputManager = SharedContext::Instance()->Get<InputManager>();
    while (const std::optional event = window->PollEvent())
    {
        inputManager->HandleInput(event.value());
    }
    SharedContext::Instance()->Get<InputManager>()->Update();
    SharedContext::Instance()->Get<EventManager>()->DispatchEvents();
}

void PureFramework::LateUpdate()
{
    SharedContext::Instance()->Get<SceneManager>()->ProcessRemoves();
}

void PureFramework::_SlowUpdate()
{
    m_lastSlowUpdate += m_elasped;
    if (m_lastSlowUpdate < sf::seconds(1.0))
        return;

    m_lastSlowUpdate -= sf::seconds(1.0);
    SharedContext::Instance()->Get<TextureManager>()->SlowUpdate();
    SharedContext::Instance()->Get<FontManager>()->SlowUpdate();
}