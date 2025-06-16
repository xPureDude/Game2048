#include "SceneDefine.hpp"

std::string_view SceneType2String(SceneType sceneType)
{
    switch (sceneType)
    {
    case SceneType::None:
        return "SceneNone";
    case SceneType::Intro:
        return "SceneIntro";
    case SceneType::MainMenu:
        return "SceneMainMenu";
    case SceneType::Setting:
        return "SceneSetting";
    case SceneType::Play:
        return "ScenePlay";
    case SceneType::Pause:
        return "ScenePause";
    case SceneType::GameOver:
        return "SceneGameOver";
    default:
        return "";
    };
}