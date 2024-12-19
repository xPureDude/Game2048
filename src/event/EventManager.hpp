#pragma once

#include "../pch.pch" // IWYU pragma: keep
#include "../scene/SceneManager.hpp"

enum class EventType
{
    WindowClosed = sf::Event::Closed,
    WindowResized = sf::Event::Resized,
    WindowLostFocus = sf::Event::LostFocus,
    WindowGainedFocus = sf::Event::GainedFocus,
    TextEntered = sf::Event::TextEntered,
    KeyPressed = sf::Event::KeyPressed,
    KeyReleased = sf::Event::KeyReleased,
    MouseWheelScrolled = sf::Event::MouseWheelScrolled,
    MouseButtonPressed = sf::Event::MouseButtonPressed,
    MouseButtonReleased = sf::Event::MouseButtonReleased,
    MouseMoved = sf::Event::MouseMoved,
    MouseEntered = sf::Event::MouseEntered,
    MouseLeft = sf::Event::MouseLeft,
    JoystickButtonPressed = sf::Event::JoystickButtonPressed,
    JoystickButtonReleased = sf::Event::JoystickButtonReleased,
    JoystickMoved = sf::Event::JoystickMoved,
    JoystickConnected = sf::Event::JoystickConnected,
    JoystickDisconnected = sf::Event::JoystickDisconnected,
    TouchBegan = sf::Event::TouchBegan,
    TouchMoved = sf::Event::TouchMoved,
    TouchEnded = sf::Event::TouchEnded,
    SensorChanged = sf::Event::SensorChanged,

    // Key hold event
    KeyBoardHold = sf::Event::Count + 1,
    MouseHold,
    JoystickHold,
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
    sf::Uint32 m_textEntered;
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
    void HandleEvent(sf::Event& event);

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
