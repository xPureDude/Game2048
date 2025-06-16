#include "BaseEvent.hpp"

namespace evt
{

std::string_view LevelToStringView(Level level)
{
    switch (level)
    {
    case Level::Global:
        return "Level::Global";
    case Level::Gui:
        return "Level::Gui";
    case Level::Input:
        return "Level::Input";
    case Level::Hud:
        return "Level::Hud";
    case Level::Scene:
        return "Level::Scene";
    default:
        return "Unknown Event Level";
    }
}
} // namespace evt
