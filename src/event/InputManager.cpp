#include "InputManager.hpp"

#include "InputBinding.hpp"

InputManager::InputManager()
    : m_isFocus(true)
{
    _InitInputBinding();
}

InputManager::~InputManager()
{
    m_bindings.clear();
}

void InputManager::Update()
{
    if (m_isFocus == false)
        return;

    // update input event binding
    if (m_bindings.contains(InputState::GlobalState))
    {
        // Key hold event
        for (auto& itBinding : m_bindings.at(InputState::GlobalState))
        {
            if (itBinding->IsTriggered())
            {
                itBinding->TriggerEvent();
            }
        }
    }

    if (m_bindings.contains(m_state))
    {
        for (auto& itBinding : m_bindings[m_state])
        {
            if (itBinding->IsTriggered())
            {
                itBinding->TriggerEvent();
            }
        }
    }
}

void InputManager::HandleInput(const sf::Event& event)
{
    if (m_bindings.contains(InputState::GlobalState))
    {
        for (auto& itBinding : m_bindings.at(InputState::GlobalState))
        {
            if (event.visit(*(itBinding)))
            {
                return;
            }
        }
    }

    if (m_bindings.contains(m_state))
    {
        for (auto& itBinding : m_bindings.at(m_state))
        {
            if (event.visit(*(itBinding)))
            {
                return;
            }
        }
    }
}

bool InputManager::CheckAction(const std::string_view& action)
{
    if (m_actions.contains(m_state))
    {
        auto& actions = m_actions.at(m_state);
        if (actions.contains(action))
        {
            return actions.at(action)->CheckTriggered();
        }
    }
    return false;
}

void InputManager::_InitInputBinding()
{
    // Global InputBinding
    {
        auto& bindings = m_bindings[InputState::GlobalState];
        bindings.push_back(std::make_shared<input::binding::MouseMove>());
        bindings.push_back(std::make_shared<input::binding::MouseButtonDown>());
        bindings.push_back(std::make_shared<input::binding::MouseButtonUp>());
        bindings.push_back(std::make_shared<input::binding::WindowClose>());
        bindings.push_back(std::make_shared<input::binding::FullscreenToggle>());
    }

    // Play InputBinding
    {
        auto& bindings = m_bindings[InputState::PlayState];
        bindings.push_back(std::make_shared<input::binding::PauseGame>());
        bindings.push_back(std::make_shared<input::binding::MoveLeft>());
        bindings.push_back(std::make_shared<input::binding::MoveRight>());
        bindings.push_back(std::make_shared<input::binding::MoveUp>());
        bindings.push_back(std::make_shared<input::binding::MoveDown>());
    }

    // Play InputAction
    {
        auto& actions = m_actions[InputState::PlayState];
        actions["MoveLeft"] = std::make_shared<input::action::MoveLeft>();
        actions["MoveRight"] = std::make_shared<input::action::MoveRight>();
        actions["MoveUp"] = std::make_shared<input::action::MoveUp>();
        actions["MoveDown"] = std::make_shared<input::action::MoveDown>();
    }
}