#include "GuiManager.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"

GuiManager::GuiManager(SharedContext* ctx)
    : SceneDependent(),
      m_ctx(ctx),
      m_target(nullptr)
{
    assert(m_ctx != nullptr);
}

GuiManager::~GuiManager()
{
    m_elements.clear();
}

void GuiManager::Update(const sf::Time& elapsed)
{
    Window* window = m_ctx->Get<Window>();
    for (auto& elem : m_elements[s_curSceneType])
    {
        elem->Update(elapsed);
    }
}

void GuiManager::Render()
{
    Window* window = m_ctx->Get<Window>();
    for (auto& elem : m_elements[s_curSceneType])
    {
        elem->Render(window->GetRenderWindow());
    }
}

void GuiManager::ClearSceneGui(SceneType type)
{
    auto iter = m_elements.find(type);
    if (iter != m_elements.end())
    {
        iter->second.clear();
        m_elements.erase(iter);
    }
}