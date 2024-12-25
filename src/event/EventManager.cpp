#include "EventManager.hpp"

#include "../core/Window.hpp"

EventManager::EventManager()
    : SceneDependent(),
      m_isFocus(true)
{
}

EventManager::~EventManager()
{
    m_bindings.clear();
}

void EventManager::Update()
{
    if (m_isFocus == false)
        return;

    // update input event binding
    auto globalBindings = m_bindings.find(SceneType::None);
    auto sceneBindings = m_bindings.find(s_curSceneType);

    // Key hold event
    for (auto& itBinding : globalBindings->second)
    {
        if (itBinding.second->IsTriggered())
        {
            itBinding.second->TriggerCallbacks();
        }
    }

    for (auto& itBinding : sceneBindings->second)
    {
        if (itBinding.second->IsTriggered())
        {
            itBinding.second->TriggerCallbacks();
        }
    }

    // update game event
    // update gui event
}

void EventManager::HandleEvent(const sf::Event& event)
{
    auto globalBindings = m_bindings.find(SceneType::None);
    auto sceneBindings = m_bindings.find(s_curSceneType);

    for (auto& itBinding : globalBindings->second)
    {
        event.visit(*(itBinding.second));
    }

    for (auto& itBinding : sceneBindings->second)
    {
        event.visit(*(itBinding.second));
    }
}

void EventManager::SetFocus(bool focus)
{
    m_isFocus = focus;
}

void EventManager::DelInputBindingCallback(SceneType type, ib::BindType bindType, const std::string_view& name)
{
    auto sceneIter = m_bindings.find(type);
    if (sceneIter != m_bindings.end())
    {
        auto bindIter = sceneIter->second.find(bindType);
        if (bindIter != sceneIter->second.end())
        {
            bindIter->second->DelBindingCallback(name);
        }
    }
}

sf::Vector2i EventManager::GetMousePosition(Window* window)
{
    if (window == nullptr)
        return sf::Mouse::getPosition();
    return sf::Mouse::getPosition(*(window->GetRenderWindow()));
}