#pragma once

#include "../pch.hpp" // IWYU pragma: keep
#include "../scene/Scene.hpp"

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

struct EventInfo
{
    EventInfo()
        : m_code(-1)
    {
    }
    EventInfo(int code)
        : m_code(code)
    {
    }
    union {
        int m_code;
    };
};

struct EventDetail
{
    EventDetail(const std::string& name);
    void Reset();

    std::string m_name;
    sf::Vector2u m_size;
    std::uint32_t m_textEntered;
    sf::Vector2f m_mousePos;
    int m_mouseWheelDelta;
    int m_keyCode;
    bool m_keyRepeat;
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct EventBinding
{
    EventBinding(const std::string& name);
    void BindEvent(EventType type, EventInfo info = EventInfo());

    int m_count;
    std::string m_name;
    Events m_events;
    EventDetail m_detail;
};

using EventBindings = std::unordered_map<std::string, EventBinding*>;
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetail*)>>;
using SceneCallbacks = std::unordered_map<SceneType, CallbackContainer>;

class Window;

class EventManager
{
public:
    EventManager();
    ~EventManager();

    void Update();
    void HandleEvent(const std::optional<sf::Event>& event);

    bool AddEventBinding(EventBinding* binding);
    bool DelEventBinding(const std::string& name);

    void SetFocus(bool focus);
    void SetCurrentSceneType(SceneType type);

    template <typename T>
    bool AddEventCallback(SceneType type, const std::string& name, void (T::*func)(EventDetail*), T* obj);
    void DelEventCallback(SceneType type, const std::string& name);

    sf::Vector2i GetMousePosition(Window* window = nullptr);

private:
    void _LoadBindings();

private:
    bool m_isFocus;
    SceneType m_curSceneType;
    EventBindings m_bindings;
    SceneCallbacks m_callbacks;
};

template <typename T>
bool EventManager::AddEventCallback(SceneType type, const std::string& name, void (T::*func)(EventDetail*), T* obj)
{
    if (m_callbacks.find(type) == m_callbacks.end())
        m_callbacks.emplace(type, CallbackContainer());

    auto& container = m_callbacks[type];

    auto temp = std::bind(func, obj, std::placeholders::_1);
    container.emplace(name, temp);
    return true;
}
