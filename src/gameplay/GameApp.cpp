#include "GameApp.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../manager/SceneManager.hpp"

GameApp::GameApp()
    : PureFramework()
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
    PureFramework::Init();
    Window* window = m_ctx.Get<Window>();
    window->Init("Game2048", sf::Vector2u(500, 600), sf::Style::Titlebar | sf::Style::Close);

    SceneManager* sceneManager = m_ctx.Get<SceneManager>();
    sceneManager->ChangeSceneTo(SceneType::Play);

    return true;
}