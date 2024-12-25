#include "GameApp.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../scene/SceneManager.hpp"

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
    window->Init("Game2048", sf::Vector2u(500, 700), sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed);

    SceneManager* sceneManager = m_ctx.Get<SceneManager>();
    sceneManager->PushScene(SceneType::Play);

    return true;
}