#pragma once

#include <string_view>

enum class SceneType
{
    None,
    Intro,
    MainMenu,
    Setting,
    Play,
    Pause,
    GameOver,
};

struct ScenePlayParam
{
    std::size_t m_rowCount{4};
    std::size_t m_colCount{4};
    bool m_isContinue{false};
};

std::string_view SceneType2String(SceneType sceneType);