#include "PureFramework.hpp"

#include "../event/EventManager.hpp"
#include "../graphics/TextureManager.hpp"
#include "../scene/SceneManager.hpp"
#include "SharedContext.hpp"
#include "Window.hpp"

#include <SFML/System.hpp>

#define FRAME_ELASPED 1000.0f / 144.0f

PureFramework::PureFramework() {}

PureFramework::~PureFramework() {}

bool PureFramework::Init()
{
    Window* window = m_ctx.Emplace<Window>();
    auto desktopSize = sf::VideoMode::getDesktopMode();
    window->Init("DesktopPet", sf::Vector2u(desktopSize.width - 1, desktopSize.height - 1), sf::Style::None, true, false);

    EventManager* eventManager = m_ctx.Emplace<EventManager>();
    window->SetEventManager(eventManager);
    TextureManager* textureManager = m_ctx.Emplace<TextureManager>();

    if (textureManager->LoadResourceInfo("Resource/Texture.xml") == false)
        return false;

    auto sceneManager = m_ctx.Emplace<SceneManager>();

    sceneManager->Init(&m_ctx);
    sceneManager->ChangeSceneTo(SceneType::Play);

    return true;
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
