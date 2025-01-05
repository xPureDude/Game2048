#include "GameApp.hpp"

#include "../core/SharedContext.hpp"
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

    SceneManager* sceneManager = m_ctx.Get<SceneManager>();
    sceneManager->PushScene(SceneType::Play);

    return true;
}