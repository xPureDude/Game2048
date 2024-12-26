#include "GuiManager.hpp"

#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"

GuiManager::GuiManager(SharedContext* ctx)
    : SceneDependent(),
      m_ctx(ctx),
      m_target(),
      m_sprite(m_target.getTexture())
{
}

GuiManager::~GuiManager()
{
    m_elements.clear();
}

void GuiManager::Update(const sf::Time& elapsed)
{
    for (auto& elem : m_elements[SceneType::None])
    {
        elem->Update(elapsed);
    }

    for (auto& elem : m_elements[s_curSceneType])
    {
        elem->Update(elapsed);
    }
}

void GuiManager::HandleInput(const sf::Event& event)
{
    for (auto& elem : m_elements[SceneType::None])
    {
        elem->HandleInput(event);
    }

    for (auto& elem : m_elements[s_curSceneType])
    {
        elem->HandleInput(event);
    }
}

void GuiManager::Render()
{
    Window* window = m_ctx->Get<Window>();
    m_target.clear(sf::Color::Transparent);
    for (auto& elem : m_elements[s_curSceneType])
    {
        elem->Render(&m_target);
    }

    for (auto& elem : m_elements[SceneType::None])
    {
        elem->Render(&m_target);
    }
    auto view = window->GetViewSpace();
    m_sprite.setPosition(view.position);

    window->Render(m_sprite);
}

void GuiManager::ClearSceneGui(SceneType type)
{
    auto iter = m_elements.find(type);
    if (iter != m_elements.end())
    {
        m_elements.erase(iter);
    }
}

std::optional<std::vector<gui::ElementPtr>> GuiManager::GetAllSceneElements(SceneType type)
{
    std::optional<std::vector<gui::ElementPtr>> elements = std::nullopt;
    if (m_elements.find(type) != m_elements.end())
    {
        elements = m_elements[type];
    }
    return elements;
}

gui::ElementPtr GuiManager::GetSceneElement(SceneType type, const std::string& name)
{
    if (m_elements.find(type) != m_elements.end())
    {
        for (auto& item : m_elements[type])
        {
            if (item->GetName() == name)
            {
                return item;
            }
        }
    }
    return nullptr;
}