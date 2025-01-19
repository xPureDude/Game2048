#include "Scene.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "SceneManager.hpp"
#include "SceneMenu.hpp"
#include "ScenePlay.hpp"

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
    default:
        return nullptr;
    }
}