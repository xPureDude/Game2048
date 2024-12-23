#pragma once

#include "../pch.hpp" // IWYU pragma: keep
#include "../scene/SceneDependent.hpp"

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
    void AddCallBack(SceneType type, std::function<void(EventDetail*)>& callback);
    void DelCallBack(SceneType type);

    int m_count;
    std::string m_name;
    Events m_events;
    EventDetail m_detail;
    std::unordered_map<SceneType, std::vector<std::function<void(EventDetail*)>>> m_callbacks;
};

using EventBindings = std::unordered_map<std::string, EventBinding*>;

class EventManager : public SceneDependent
{
public:
    EventManager();
    ~EventManager();

    void Update();
    void HandleEvent(const sf::Event& event);

    bool AddEventBinding(EventBinding* binding);
    bool DelEventBinding(const std::string& name);

    void SetFocus(bool focus);

    template <typename T>
    bool AddEventCallback(SceneType type, const std::string& name, void (T::*func)(EventDetail*), T* obj);
    void DelEventCallback(SceneType type, const std::string& name);

    sf::Vector2i GetMousePosition(Window* window = nullptr);

private:
    void _LoadBindings();

private:
    bool m_isFocus;
    EventBindings m_bindings;
};

template <typename T>
bool EventManager::AddEventCallback(SceneType type, const std::string& name, void (T::*func)(EventDetail*), T* obj)
{
    auto binding = m_bindings.find(name);
    if (binding == m_bindings.end())
        return false;

    auto callback = std::bind(func, obj, std::placeholders::_1);
    binding->second->AddCallBack(type, callback);
    return true;
}
