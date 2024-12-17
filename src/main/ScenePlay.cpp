#include "ScenePlay.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../event/EventManager.hpp"
#include "../scene/SceneManager.hpp"

ScenePlay::ScenePlay()
{
}

ScenePlay::~ScenePlay() {}

bool ScenePlay::OnCreate(SceneManager* manager)
{
    if (Scene::OnCreate(manager) == false)
        return false;

    EventManager* eventManager = m_sceneManager->GetSharedContext()->Get<EventManager>();
    eventManager->AddEventCallback(SceneType::Play, "Play_Pause", &ScenePlay::_Pause, this);

    return true;
}

void ScenePlay::OnDestroy()
{
    EventManager* eventManager = m_sceneManager->GetSharedContext()->Get<EventManager>();
    eventManager->DelEventCallback(SceneType::Play, "Play_Pause");
}

void ScenePlay::Update(const sf::Time& elapsed)
{
    // Check Window Focus
}

void ScenePlay::Render(Window* window)
{
}

void ScenePlay::OnEnter() {}

void ScenePlay::OnLeave() {}

void ScenePlay::_Pause(EventDetail* detail)
{
    m_sceneManager->ChangeSceneTo(SceneType::Paused);
}