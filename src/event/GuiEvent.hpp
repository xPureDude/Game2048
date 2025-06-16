#pragma once

#include "BaseEvent.hpp"

namespace evt
{

struct BeginGame : public GuiBase
{
    EVENT_STATIC_NAME(BeginGame);
};

struct QuitGame : public GuiBase
{
    EVENT_STATIC_NAME(QuitGame);
};

struct RetryGame : public GuiBase
{
    EVENT_STATIC_NAME(RetryGame);
};

struct PauseGame : public GuiBase
{
    EVENT_STATIC_NAME(PauseGame);
};

struct ContinueGame : public GuiBase
{
    EVENT_STATIC_NAME(ContinueGame);
};

struct BackToMainMenu : public GuiBase
{
    EVENT_STATIC_NAME(BackToMainMenu);
};

struct OpenSetting : public GuiBase
{
    EVENT_STATIC_NAME(OpenSetting);
};

} // namespace evt