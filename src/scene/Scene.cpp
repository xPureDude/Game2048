#include "Scene.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "SceneManager.hpp"
#include "SceneMenu.hpp"
#include "ScenePlay.hpp"

Scene::Scene()
    : m_updateTransparent(false),
      m_renderTransparent(false),
      m_sceneManager(nullptr)
{
}

Scene::~Scene() {}

bool Scene::OnCreate(SceneManager* manager)
{
    if (manager == nullptr)
        return false;

    m_view = manager->GetSharedContext()->Get<Window>()->GetDefaultView();
    m_sceneManager = manager;

    return true;
}

std::shared_ptr<Scene> SceneFactory::CreateScene(SceneType sceneType)
{
    if (m_scenes.find(sceneType) == m_scenes.end())
    {
        switch (sceneType)
        {
        case SceneType::Play:
        {
            m_scenes[sceneType] = std::make_shared<ScenePlay>();
            break;
        }
        case SceneType::MainMenu:
        {
            m_scenes[sceneType] = std::make_shared<SceneMenu>();
            break;
        }
        default:
            return nullptr;
        }
    }

    return m_scenes[sceneType];
}

SceneFactory::~SceneFactory()
{
    m_scenes.clear();
}