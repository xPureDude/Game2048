#pragma once

#include "../pch.hpp" // IWYU pragma: keep
#include "../scene/SceneDependent.hpp"
#include "InputBinding.hpp"

enum class EventType
{
    // Key hold event
    KeyBoardHold,
    MouseHold,
    JoystickHold,

    // Gui Event
    GuiHovered,
    GuiPressed,
    GuiReleased,
    GuiDetached,
};

class EventManager : public SceneDependent
{
public:
    EventManager();
    ~EventManager();

    void Update();
    void HandleEvent(const sf::Event& event);

    void SetFocus(bool focus);

    template <typename T>
    bool AddInputBindingCallback(SceneType type, ib::BindType bindType, const std::string_view& name, void (T::*func)(), T* obj);
    void DelInputBindingCallback(SceneType type, ib::BindType bindType, const std::string_view& name);

    sf::Vector2i GetMousePosition(Window* window = nullptr);

private:
    bool m_isFocus;
    InputBindingFactory m_inputBindingFactory;
    std::map<SceneType, std::map<ib::BindType, ib::BindingPtr>> m_bindings;
};

template <typename T>
bool EventManager::AddInputBindingCallback(SceneType sceneType, ib::BindType bindType, const std::string_view& name, void (T::*func)(), T* obj)
{
    if (m_bindings.find(sceneType) == m_bindings.end())
    {
        m_bindings.emplace(sceneType, std::map<ib::BindType, ib::BindingPtr>());
    }

    auto& sceneBindings = m_bindings[sceneType];

    if (sceneBindings.find(bindType) == sceneBindings.end())
    {
        // Try create new Binding;
        auto newBindPtr = m_inputBindingFactory.CreateInputBinding(bindType);
        if (newBindPtr == nullptr)
        {
            return false;
        }
        sceneBindings.emplace(bindType, newBindPtr);
    }

    auto& bindPtr = sceneBindings[bindType];

    // auto callback = std::bind(func, obj, std::placeholders::_1);
    auto callback = std::bind(func, obj);
    bindPtr->AddBindingCallback(name, callback);
    return true;
}
