#include "Scene.hpp"

#include "../manager/SceneManager.hpp"
#include "SharedContext.hpp"
#include "Window.hpp"

Scene::Scene()
    : m_isTransparent(false),
      m_isTranscent(false),
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
