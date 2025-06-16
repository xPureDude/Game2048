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

void InputManager::_InitInputBinding()
{
    // Global InputBinding
    {
        auto& bindings = m_bindings[InputState::GlobalState];
        bindings.push_back(std::make_shared<input::MouseMove>());
        bindings.push_back(std::make_shared<input::MouseButtonDown>());
        bindings.push_back(std::make_shared<input::MouseButtonUp>());
        bindings.push_back(std::make_shared<input::WindowClose>());
        bindings.push_back(std::make_shared<input::FullscreenToggle>());
    }

    // Play InputBinding
    {
        auto& bindings = m_bindings[InputState::PlayState];
        bindings.push_back(std::make_shared<input::MoveLeft>());
        bindings.push_back(std::make_shared<input::MoveRight>());
        bindings.push_back(std::make_shared<input::MoveUp>());
        bindings.push_back(std::make_shared<input::MoveDown>());
    }
}