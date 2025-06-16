#include "EventManager.hpp"

#include "ScopedEventListener.hpp"

EventManager::EventManager()
    : m_nextToken(0)
{
}

void EventManager::DispatchEvents()
{
    std::swap(m_eventQueue, m_backupQueue);
    for (auto& info : m_eventQueue)
    {
        _DispatchSingleEvent(info);
    }
    m_eventQueue.clear();
}

void EventManager::_DispatchSingleEvent(EventInfo& info)
{
    if (!m_eventListeners.contains(info.m_level))
        return;

    for (auto [_, listener] : m_eventListeners.at(info.m_level))
    {
        if (info.m_level != evt::Level::Global && !listener->GetActivate())
        {
            continue;
        }

        listener->HandleEvent(info.m_index, info.m_event.get());
        if (info.m_event->m_isProcessed)
        {
            break;
        }
    }
}

std::size_t EventManager::_SubscribeListener(evt::Level level, ScopedEventListener* listener)
{
    if (!m_eventListeners.contains(level))
    {
        m_eventListeners.try_emplace(level, std::unordered_map<std::size_t, ScopedEventListener*>());
    }

    m_eventListeners[level][m_nextToken] = listener;

    return m_nextToken++;
}

void EventManager::_UnSubscribeListener(evt::Level level, std::size_t token)
{
    if (!m_eventListeners.contains(level))
    {
        return;
    }

    if (!m_eventListeners.at(level).contains(token))
    {
        return;
    }

    m_eventListeners.at(level).erase(token);
    if (m_eventListeners.at(level).empty())
    {
        m_eventListeners.erase(level);
    }
}