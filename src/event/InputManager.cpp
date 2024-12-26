#include "InputManager.hpp"

InputManager::InputManager()
    : SceneDependent(),
      m_isFocus(true)
{
}

InputManager::~InputManager()
{
    m_bindings.clear();
}

void InputManager::Update(const sf::Time& elapsed)
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

void InputManager::HandleInput(const sf::Event& event)
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

void InputManager::SetFocus(bool focus)
{
    m_isFocus = focus;
}

void InputManager::DelInputBindingCallback(SceneType type, ib::BindType bindType, const std::string_view& name)
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