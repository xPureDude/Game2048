#include "Scene.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../pch.hpp" // IWYU pragma: keep
#include "SceneGameOver.hpp"
#include "SceneManager.hpp"
#include "SceneMenu.hpp"
#include "ScenePlay.hpp"

std::string_view TranslateSceneTypeToString(SceneType type)
{
    std::string_view ret;
    switch (type)
    {
    case SceneType::Intro:
        ret = "Intro";
        break;
    case SceneType::MainMenu:
        ret = "MainMenu";
        break;
    case SceneType::Play:
        ret = "Play";
        break;
    case SceneType::Paused:
        ret = "Paused";
        break;
    case SceneType::GameOver:
        ret = "GameOver";
        break;

    default:
        ret = "None";
    }
    return ret;
}

Scene::Scene(SceneManager* manager)
    : m_updateTransparent(false),
      m_renderTransparent(false),
      m_sceneManager(manager)
{
    m_view = manager->GetSharedContext()->Get<Window>()->GetDefaultView();
    m_sceneManager = manager;
}

Scene::~Scene() {}

std::shared_ptr<Scene> SceneFactory::CreateScene(SceneType sceneType, SceneManager* manager)
{
    switch (sceneType)
    {
    case SceneType::Play:
    {
        return std::make_shared<ScenePlay>(manager);
    }
    case SceneType::MainMenu:
    {
        return std::make_shared<SceneMenu>(manager);
    }
    case SceneType::GameOver:
        return std::make_shared<SceneGameOver>(manager);
    default:
        return nullptr;
    }
}