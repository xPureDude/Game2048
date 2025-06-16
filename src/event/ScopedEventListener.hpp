#pragma once

#include <typeindex>
#include <unordered_map>

#include "BaseEvent.hpp"
#include "common/Utility.hpp"

class ScopedEventListener
{
    struct CallbackInfo
    {
        std::string_view m_name;
        EventCallbackType m_func;
    };

    struct TokenCount
    {
        std::size_t m_token{0};
        std::int32_t m_count{0};
    };

public:
    ScopedEventListener();
    virtual ~ScopedEventListener();

    void HandleEvent(std::type_index typeIndex, evt::Base* e);

    template <typename EventT>
    void Subscribe(EventCallbackType callback);

    template <typename EventT>
    void UnSubscribe();

    void SetActivate(bool flag) { m_isActivate = flag; }

    bool GetActivate() { return m_isActivate; }

private:
    void _DoSubscribe(std::type_index index, evt::Level level, EventCallbackType callback);
    void _DoUnSubscribe(std::type_index index, evt::Level level);

private:
    bool m_isActivate;

    // <EventType, Callback>
    std::unordered_map<std::type_index, EventCallbackType> m_eventCallbacks;
    // <EventLevel, (Token, Ref Count)>
    std::unordered_map<evt::Level, TokenCount> m_busTokens;
};

template <typename EventT>
void ScopedEventListener::Subscribe(EventCallbackType callback)
{
    auto index = std::type_index(typeid(EventT));
    auto level = EventT::GetEventLevel();
    _DoSubscribe(index, level, callback);

    DBG("ScopedEventListener::Subscribe, EventName: {}", EventT::GetEventName());
}

template <typename EventT>
void ScopedEventListener::UnSubscribe()
{
    auto index = std::type_index(typeid(EventT));
    auto level = EventT::GetEventLevel();
    _DoUnSubscribe(index, level);

    DBG("ScopedEventListener::UnSubscribe, EventName: {}", EventT::GetEventName());
}