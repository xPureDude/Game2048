#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "BaseEvent.hpp"

class EventManager
{
    struct EventInfo
    {
        std::unique_ptr<evt::Base> m_event;
        std::type_index m_index;
        evt::Level m_level;
    };
    friend class ScopedEventListener;

public:
    EventManager();
    ~EventManager() {};

    template <typename EventT>
    void ProcessEvent(evt::Base* e);

    template <typename EventT>
    EventT* PushEvent();

    void DispatchEvents();

private:
    void _DispatchSingleEvent(EventInfo& info);
    std::size_t _SubscribeListener(evt::Level level, ScopedEventListener* listener);
    void _UnSubscribeListener(evt::Level level, std::size_t token);

private:
    std::size_t m_nextToken;
    std::vector<EventInfo> m_eventQueue;
    std::vector<EventInfo> m_backupQueue;
    std::unordered_map<evt::Level, std::unordered_map<std::size_t, ScopedEventListener*>> m_eventListeners;
};

template <typename EventT>
void EventManager::ProcessEvent(evt::Base* e)
{
    auto index = std::type_index(typeid(EventT));
    auto level = EventT::GetEventLevel();
    EventInfo info = {std::make_unique<EventT>(), index, level};

    _DispatchSingleEvent(info);
}

template <typename EventT>
EventT* EventManager::PushEvent()
{
    auto ptr = std::make_unique<EventT>();
    auto index = std::type_index(typeid(EventT));
    auto level = EventT::GetEventLevel();

    auto rawPtr = ptr.get();
    m_backupQueue.emplace_back(std::move(ptr), index, level);

    return rawPtr;
}