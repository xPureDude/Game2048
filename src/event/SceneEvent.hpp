#pragma once

#include "BaseEvent.hpp"

namespace evt
{

struct ScoreChange : public SceneBase
{
    EVENT_STATIC_NAME(ScoreChange);

    std::uint32_t m_curScore{0};
};

struct GameOver : public SceneBase
{
    EVENT_STATIC_NAME(GameOver);

    bool m_isWin{false};
};
} // namespace evt