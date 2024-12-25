#include "ScenePlay.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../event/EventManager.hpp"
#include "SceneManager.hpp"

ScenePlay::ScenePlay()
{
}

ScenePlay::~ScenePlay() {}

bool ScenePlay::OnCreate(SceneManager* manager)
{
    if (Scene::OnCreate(manager) == false)
        return false;

    EventManager* eventManager = m_sceneManager->GetSharedContext()->Get<EventManager>();
    eventManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveLeft, "ScenePlay_MoveLeft", &ScenePlay::_OnMoveLeft, this);
    eventManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveRight, "ScenePlay_MoveRight", &ScenePlay::_OnMoveRight, this);
    eventManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveUp, "ScenePlay_MoveUp", &ScenePlay::_OnMoveUp, this);
    eventManager->AddInputBindingCallback(SceneType::Play, ib::BindType::MoveDown, "ScenePlay_MoveDown", &ScenePlay::_OnMoveDown, this);

    Window* window = m_sceneManager->GetSharedContext()->Get<Window>();
    auto windowSize = window->GetSize();
    m_game2048.SetBoardSize(500);
    m_game2048.SetPosition({0, (float)(windowSize.y - m_game2048.GetBoardSize())});

    return true;
}

void ScenePlay::OnDestroy()
{
    EventManager* eventManager = m_sceneManager->GetSharedContext()->Get<EventManager>();
    eventManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveLeft, "ScenePlay_MoveLeft");
    eventManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveRight, "ScenePlay_MoveRight");
    eventManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveUp, "ScenePlay_MoveUp");
    eventManager->DelInputBindingCallback(SceneType::Play, ib::BindType::MoveDown, "ScenePlay_MoveDown");
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

void ScenePlay::_OnMoveLeft()
{
    m_game2048.OnMoveLeft();
}

void ScenePlay::_OnMoveRight()
{
    m_game2048.OnMoveRight();
}

void ScenePlay::_OnMoveUp()
{
    m_game2048.OnMoveUp();
}

void ScenePlay::_OnMoveDown()
{
    m_game2048.OnMoveDown();
}