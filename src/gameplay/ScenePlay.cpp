#include "ScenePlay.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../manager/EventManager.hpp"
#include "../manager/SceneManager.hpp"

ScenePlay::ScenePlay()
{
}

ScenePlay::~ScenePlay() {}

bool ScenePlay::OnCreate(SceneManager* manager)
{
    if (Scene::OnCreate(manager) == false)
        return false;

    EventManager* eventManager = m_sceneManager->GetSharedContext()->Get<EventManager>();
    eventManager->AddEventCallback(SceneType::Play, "Move_Left", &ScenePlay::_OnMoveLeft, this);
    eventManager->AddEventCallback(SceneType::Play, "Move_Right", &ScenePlay::_OnMoveRight, this);
    eventManager->AddEventCallback(SceneType::Play, "Move_Up", &ScenePlay::_OnMoveUp, this);
    eventManager->AddEventCallback(SceneType::Play, "Move_Down", &ScenePlay::_OnMoveDown, this);

    m_game2048.SetPosition({0, 100});

    return true;
}

void ScenePlay::OnDestroy()
{
    EventManager* eventManager = m_sceneManager->GetSharedContext()->Get<EventManager>();
    eventManager->DelEventCallback(SceneType::Play, "Move_Left");
    eventManager->DelEventCallback(SceneType::Play, "Move_Right");
    eventManager->DelEventCallback(SceneType::Play, "Move_Up");
    eventManager->DelEventCallback(SceneType::Play, "Move_Down");
}

void ScenePlay::Update(const sf::Time& elapsed)
{
    // Check Window Focus
    m_game2048.Update(elapsed);
}

void ScenePlay::Render(Window* window)
{
    m_game2048.Render(window);
}

void ScenePlay::OnEnter()
{
    m_game2048.OnNewGame(4, 4);
}

void ScenePlay::OnLeave() {}

void ScenePlay::_OnMoveLeft(EventDetail* detail)
{
    m_game2048.OnMoveLeft();
}

void ScenePlay::_OnMoveRight(EventDetail* detail)
{
    m_game2048.OnMoveRight();
}

void ScenePlay::_OnMoveUp(EventDetail* detail)
{
    m_game2048.OnMoveUp();
}

void ScenePlay::_OnMoveDown(EventDetail* detail)
{
    m_game2048.OnMoveDown();
}