#include "Scene.hpp"

#include "core/SharedContext.hpp"
#include "core/Window.hpp"

std::string_view TranslateSceneTypeToStringView(SceneType type)
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

Scene::Scene()
    : m_updateTransparent(false),
      m_renderTransparent(false)
{
    m_view = SharedContext::Instance()->Get<Window>()->GetDefaultView();
}

Scene::~Scene() {}