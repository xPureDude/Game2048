#pragma once

#include <unordered_map>
#include <vector>

#include "common/Utility.hpp"

enum class EventType
{
};

class EventBase;

class EventManager
{
public:
    EventManager();
    ~EventManager();

private:
    std::vector<EventBase*> m_eventQueue;
    std::unordered_map<EventType, std::unordered_map<std::string, CallbackType>> m_eventTriggers;
};