#pragma once

#include "ScopedEventListener.hpp"
#include "common/Utility.hpp"

class EventListenable
{
public:
    template <typename EventT>
    void SubscribeEvent(EventCallbackType callback)
    {
        m_eventListener.Subscribe<EventT>(callback);
    }

    template <typename EventT>
    void UnsubscribeEvent()
    {
        m_eventListener.UnSubscribe<EventT>();
    }

protected:
    ScopedEventListener m_eventListener;
};