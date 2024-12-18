#include "PureFramework.hpp"

#include "../manager/EventManager.hpp"
#include "../manager/SceneManager.hpp"
#include "../manager/TextureManager.hpp"
#include "SharedContext.hpp"
#include "Window.hpp"

#include <SFML/System.hpp>

#define FRAME_ELASPED 1000.0f / 144.0f

PureFramework::PureFramework() {}

PureFramework::~PureFramework() {}

void PureFramework::Init()
{
    m_ctx.Emplace<EventManager>([](void* obj) { delete (EventManager*)obj; });
    m_ctx.Emplace<Window>([](void* obj) { delete (Window*)obj; }, m_ctx.Get<EventManager>());
    m_ctx.Emplace<TextureManager>([](void* obj) { delete (TextureManager*)obj; });
    m_ctx.Emplace<SceneManager>([](void* obj) { delete (SceneManager*)obj; }, &m_ctx);
}

void PureFramework::UnInit()
{
    m_ctx.Release<SceneManager>();
    m_ctx.Release<TextureManager>();
    m_ctx.Release<Window>();
    m_ctx.Release<EventManager>();
}

void PureFramework::Run()
{
    sf::Time frameElapsed = sf::milliseconds(FRAME_ELASPED);
    m_clock.restart();
    while (m_ctx.Get<Window>()->IsClose() == false)
    {
        m_elasped = m_clock.getElapsedTime();
        if (m_elasped < frameElapsed)
        {
            sf::sleep(frameElapsed - m_elasped);
        }
        m_elasped = m_clock.restart();

        Update();
        HandleEvent();
        Render();
        LateUpdate();
    }
}

void PureFramework::Update()
{
    m_ctx.Get<Window>()->Update();
    m_ctx.Get<SceneManager>()->Update(m_elasped);
}

void PureFramework::Render()
{
    m_ctx.Get<Window>()->BeginRender();
    m_ctx.Get<SceneManager>()->Render();
    m_ctx.Get<Window>()->EndRender();
}

void PureFramework::HandleEvent()
{
    m_ctx.Get<Window>()->HandleEvent();
}

void PureFramework::LateUpdate()
{
    m_ctx.Get<SceneManager>()->ProcessRemoves();
}
