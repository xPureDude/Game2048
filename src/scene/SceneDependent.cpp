#include "SceneDependent.hpp"

#include "SceneGameOver.hpp"
#include "SceneMenu.hpp"
#include "ScenePlay.hpp"

std::vector<SceneDependent*> SceneDependent::s_instances;
SceneType SceneDependent::s_curSceneType = SceneType::None;

SceneDependent::SceneDependent()
{
    s_instances.push_back(this);
}

SceneDependent::~SceneDependent()
{
}

void SceneDependent::ChangeCurrentSceneType(SceneType newType)
{
    s_curSceneType = newType;
}

std::shared_ptr<Scene> SceneFactory::CreateScene(SceneType sceneType)
{
    switch (sceneType)
    {
    case SceneType::Play:
    {
        return std::make_shared<ScenePlay>();
    }
    case SceneType::MainMenu:
    {
        return std::make_shared<SceneMenu>();
    }
    case SceneType::GameOver:
    {
        return std::make_shared<SceneGameOver>();
    }
    default:
        return nullptr;
    }
}