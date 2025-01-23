#include "SceneGameOver.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../gui/GuiManager.hpp"
#include "../pch.hpp"
#include "SceneManager.hpp"

SceneGameOver::SceneGameOver(SceneManager* manager)
    : Scene(manager)
{
    m_renderTransparent = true;
}

SceneGameOver::~SceneGameOver()
{
}

bool SceneGameOver::OnCreate()
{
    if (!_InitGui())
        return false;

    return true;
}

void SceneGameOver::OnDestroy()
{
    GuiManager* guiManager = m_sceneManager->GetSharedContext()->Get<GuiManager>();
    guiManager->ClearSceneGui(SceneType::GameOver);
}

void SceneGameOver::Update(const sf::Time& elasped)
{
}

void SceneGameOver::Render(Window* window)
{
}

void SceneGameOver::OnEnter(const std::any& param)
{
}

void SceneGameOver::OnLeave()
{
}

bool SceneGameOver::_InitGui()
{

    return true;
}

void SceneGameOver::_OnNewGame(const std::any& param)
{
}

void SceneGameOver::_OnContinuePlay(const std::any& param)
{
}

void SceneGameOver::_OnQuitGame(const std::any& param)
{
}
