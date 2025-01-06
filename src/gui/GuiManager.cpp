#include "GuiManager.hpp"

#include "../core/ConfigManager.hpp"
#include "../core/SharedContext.hpp"
#include "../core/Window.hpp"
#include "../gui/Widget.hpp"
#include "../pch.hpp"

GuiManager::GuiManager(SharedContext* ctx)
    : SceneDependent(),
      SharedContextDependent(ctx),
      m_target({m_ctx->Get<ConfigManager>()->GetWindowConfig().m_width, m_ctx->Get<ConfigManager>()->GetWindowConfig().m_height}),
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
        if (elem->HandleInput(event))
        {
            return;
        }
    }

    for (auto& elem : m_elements[s_curSceneType])
    {
        if (elem->HandleInput(event))
        {
            return;
        }
    }
}

void GuiManager::Render()
{
    Window* window = m_ctx->Get<Window>();
    for (auto& elem : m_elements[s_curSceneType])
    {
        elem->Render(&m_target);
    }

    for (auto& elem : m_elements[SceneType::None])
    {
        elem->Render(&m_target);
    }
    m_target.display();
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

void GuiManager::AddSceneGui(SceneType type, std::shared_ptr<gui::Element> elem)
{
    m_elements[type].emplace_back(elem);
}

void GuiManager::AddSceneGui(SceneType type, std::vector<std::shared_ptr<gui::Element>>& elem)
{
    for (auto& elemPtr : elem)
    {
        m_elements[type].emplace_back(elemPtr);
    }
}

std::optional<std::vector<std::shared_ptr<gui::Element>>> GuiManager::GetAllSceneElements(SceneType type)
{
    std::optional<ElementContainer> elements = std::nullopt;
    if (m_elements.find(type) != m_elements.end())
    {
        elements = m_elements[type];
    }
    return elements;
}

std::shared_ptr<gui::Element> GuiManager::GetSceneElementByName(SceneType type, const std::string& name)
{
    if (m_elements.find(type) != m_elements.end())
    {
        for (auto& item : m_elements[type])
        {
            if (item->GetName() == name)
            {
                return item;
            }
            else if (auto widget = std::dynamic_pointer_cast<gui::Widget>(item))
            {
                if (auto child = widget->FindChild(name))
                {
                    return child;
                }
            }
        }
    }
    return nullptr;
}