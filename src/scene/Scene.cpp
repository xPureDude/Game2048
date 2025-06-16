#include "Scene.hpp"

#include "core/SharedContext.hpp"
#include "core/Window.hpp"

Scene::Scene()
    : m_updateTransparent(false),
      m_renderTransparent(false)
{
    m_view = SharedContext::Instance()->Get<Window>()->GetDefaultView();
}

Scene::~Scene() {}

void Scene::OnEnter(const std::any& param)
{
    m_eventListener.SetActivate(true);
}

void Scene::OnLeave()
{
    m_eventListener.SetActivate(false);
}