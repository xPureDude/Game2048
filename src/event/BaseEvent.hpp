#pragma once

#include <string_view>

namespace evt
{

enum class Level
{
    Global,
    Input,
    Gui,
    Hud,
    Scene
};

std::string_view LevelToStringView(Level level);

#define EVENT_STATIC_LEVEL(x)    \
    static Level GetEventLevel() \
    {                            \
        return x;                \
    }

#define EVENT_STATIC_NAME(x)               \
    static std::string_view GetEventName() \
    {                                      \
        return #x;                         \
    }

struct Base
{
    virtual ~Base() = default;

    bool m_isProcessed{false};
};

struct GlobalBase : public Base
{
    EVENT_STATIC_LEVEL(Level::Global);
};

struct InputBase : public Base
{
    EVENT_STATIC_LEVEL(Level::Input);
};

struct GuiBase : public Base
{
    EVENT_STATIC_LEVEL(Level::Gui)
};

struct SceneBase : public Base
{
    EVENT_STATIC_LEVEL(Level::Scene)
};

} // namespace evt
