#pragma once

#include "BaseEvent.hpp"

namespace evt
{

struct FullscreenToggle : public GlobalBase
{
    EVENT_STATIC_NAME(FullscreenToggle);
};

struct WindowClose : public GlobalBase
{
    EVENT_STATIC_NAME(WindowClose);
};

struct LanguageChanged : public GlobalBase
{
    EVENT_STATIC_NAME(LanguageChanged);
    std::string m_language;
};

} // namespace evt