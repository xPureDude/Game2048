#include "EventManager.hpp"

#include "../core/Window.hpp"

EventDetail::EventDetail(const std::string& name)
    : m_name(name)
{
    Reset();
}

void EventDetail::Reset()
{
    m_mouseWheelDelta = 0;
    m_keyCode = -1;
    m_keyRepeat = false;
}

EventBinding::EventBinding(const std::string& name)
    : m_count(0),
      m_name(name),
      m_detail(name)
{
}

void EventBinding::BindEvent(EventType type, EventInfo info)
{
    m_events.emplace_back(type, info);
}

EventManager::EventManager()
    : m_isFocus(true),
      m_curSceneType(SceneType::None)
{
    _LoadBindings();
}

EventManager::~EventManager()
{
    for (auto& iter : m_bindings)
    {
        delete iter.second;
        iter.second = nullptr;
    }
}

void EventManager::Update()
{
    if (m_isFocus == false)
        return;

    // Key hold event
    for (auto& bindIter : m_bindings)
    {
        auto binding = bindIter.second;
        for (auto& eventIter : binding->m_events)
        {
            switch (eventIter.first)
            {
            case EventType::KeyBoardHold:
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(eventIter.second.m_code)) == true)
                {
                    binding->m_detail.m_keyCode = eventIter.second.m_code;
                    ++(binding->m_count);
                }
            }
            break;

            case EventType::MouseHold:
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button(eventIter.second.m_code)) == true)
                {
                    binding->m_detail.m_keyCode = eventIter.second.m_code;
                    ++(binding->m_count);
                }
            }
            break;

            case EventType::JoystickHold:
            {
            }
            break;

            default:
            {
            }
            break;
            }
        }

        if (binding->m_count == binding->m_events.size())
        {
            auto sceneCallbacks = m_callbacks.find(m_curSceneType);
            auto globalCallbacks = m_callbacks.find(SceneType::None);

            if (sceneCallbacks != m_callbacks.end())
            {
                auto callbackIter = sceneCallbacks->second.find(binding->m_name);
                if (callbackIter != sceneCallbacks->second.end())
                {
                    // DBG("EventManager::Update, SceneCallback {} invoked", binding->m_name);
                    callbackIter->second(&binding->m_detail);
                }
            }

            if (globalCallbacks != m_callbacks.end())
            {
                auto callbackIter = globalCallbacks->second.find(binding->m_name);
                if (callbackIter != globalCallbacks->second.end())
                {
                    // DBG("EventManager::Update, GlobalCallback {} invoked", binding->m_name);
                    callbackIter->second(&binding->m_detail);
                }
            }
        }
        binding->m_count = 0;
        binding->m_detail.Reset();
    }
}

void EventManager::HandleEvent(sf::Event& event)
{
    // Regular event
    for (auto& bindIter : m_bindings)
    {
        auto binding = bindIter.second;
        for (auto& eventIter : binding->m_events)
        {
            auto eventType = (EventType)event.type;
            if (eventIter.first != eventType)
                continue;

            switch (eventType)
            {
            case EventType::KeyPressed:
            case EventType::KeyReleased:
            {
                if (eventIter.second.m_code == event.key.code)
                {
                    binding->m_detail.m_keyCode = eventIter.second.m_code;
                    ++(binding->m_count);
                }
                break;
            }

            case EventType::MouseButtonPressed:
            case EventType::MouseButtonReleased:
            {
                if (eventIter.second.m_code == event.mouseButton.button)
                {
                    binding->m_detail.m_mousePos.x = event.mouseButton.x;
                    binding->m_detail.m_mousePos.y = event.mouseButton.y;
                    binding->m_detail.m_keyCode = eventIter.second.m_code;
                    ++(binding->m_count);
                }
                break;
            }

            case EventType::MouseWheelScrolled:
            {
                binding->m_detail.m_mouseWheelDelta = event.mouseWheel.delta;
                ++(binding->m_count);
                break;
            }

            case EventType::MouseMoved:
            {
                binding->m_detail.m_mousePos.x = (float)event.mouseMove.x;
                binding->m_detail.m_mousePos.y = (float)event.mouseMove.y;
                ++(binding->m_count);
                break;
            }

            case EventType::WindowResized:
            {
                binding->m_detail.m_size.x = event.size.width;
                binding->m_detail.m_size.y = event.size.height;
                ++(binding->m_count);
                break;
            }

            case EventType::TextEntered:
            {
                binding->m_detail.m_textEntered = event.text.unicode;
                ++(binding->m_count);
                break;
            }

            default:
                break;
            }
        }
    }
}

bool EventManager::AddEventBinding(EventBinding* binding)
{
    if (binding == nullptr)
        return false;
    if (m_bindings.find(binding->m_name) != m_bindings.end())
        return false;

    m_bindings.emplace(binding->m_name, binding);

    return true;
}

bool EventManager::DelEventBinding(const std::string& name)
{
    auto iter = m_bindings.find(name);
    if (iter == m_bindings.end())
        return false;
    delete iter->second;
    m_bindings.erase(name);
    return true;
}

void EventManager::SetFocus(bool focus)
{
    m_isFocus = focus;
}

void EventManager::SetCurrentSceneType(SceneType type)
{
    m_curSceneType = type;
}

void EventManager::DelEventCallback(SceneType type, const std::string& name)
{
    auto sceneIter = m_callbacks.find(type);
    if (sceneIter != m_callbacks.end())
    {
        auto callbackIter = sceneIter->second.find(name);
        if (callbackIter != sceneIter->second.end())
        {
            sceneIter->second.erase(callbackIter);
        }
    }
}

sf::Vector2i EventManager::GetMousePosition(Window* window)
{
    if (window == nullptr)
        return sf::Mouse::getPosition();
    return sf::Mouse::getPosition(*(window->GetRenderWindow()));
}

void EventManager::_LoadBindings()
{
    auto eventBind = [this](const std::string& name, EventType eventType, EventInfo info) {
        auto eventBinding = new EventBinding(name);
        eventBinding->BindEvent(eventType, info);
        m_bindings.emplace(name, eventBinding);
    };

    eventBind("Window_Close", EventType::WindowClosed, 0);
    eventBind("Mouse_Move", EventType::MouseMoved, 0);
    eventBind("Mouse_Left_Pressed", EventType::MouseButtonPressed, sf::Mouse::Left);
    eventBind("Mouse_Left_Release", EventType::MouseButtonReleased, sf::Mouse::Left);

    eventBind("Move_Left", EventType::KeyPressed, sf::Keyboard::Left);
    eventBind("Move_Right", EventType::KeyPressed, sf::Keyboard::Right);
    eventBind("Move_Up", EventType::KeyPressed, sf::Keyboard::Up);
    eventBind("Move_Down", EventType::KeyPressed, sf::Keyboard::Down);
}
