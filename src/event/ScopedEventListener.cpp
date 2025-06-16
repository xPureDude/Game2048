#include "ScopedEventListener.hpp"

#include "common/Log.hpp"
#include "core/SharedContext.hpp"
#include "event/EventManager.hpp"

ScopedEventListener::ScopedEventListener()
    : m_isActivate(true)
{
}

ScopedEventListener::~ScopedEventListener()
{
    m_eventCallbacks.clear();
    for (auto& [level, busCount] : m_busTokens)
    {
        SharedContext::Instance()->Get<EventManager>()->_UnSubscribeListener(level, busCount.m_token);
    }
}

void ScopedEventListener::HandleEvent(std::type_index typeIndex, evt::Base* e)
{
    if (m_eventCallbacks.contains(typeIndex))
    {
        auto& callback = m_eventCallbacks.at(typeIndex);
        callback(e);

        if (e->m_isProcessed)
        {
            return;
        }
    }
}

void ScopedEventListener::_DoSubscribe(std::type_index index, evt::Level level, EventCallbackType callback)
{
    if (!m_busTokens.contains(level))
    {
        auto token = SharedContext::Instance()->Get<EventManager>()->_SubscribeListener(level, this);
        m_busTokens.try_emplace(level, TokenCount{token, 0});
    }

    if (auto ret = m_eventCallbacks.try_emplace(index, callback); !ret.second)
    {
        DBG("ScopedEventListener::_DoSubscribe, Duplicate Subscribe EventType to same EventListener");
    }
}

void ScopedEventListener::_DoUnSubscribe(std::type_index index, evt::Level level)
{
    if (!m_eventCallbacks.contains(index))
    {
        return;
    }
    m_eventCallbacks.erase(index);

    m_busTokens.at(level).m_count--;
    if (m_busTokens.at(level).m_count == 0)
    {
        m_busTokens.erase(level);
    }
}