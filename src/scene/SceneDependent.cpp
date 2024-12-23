#include "SceneDependent.hpp"

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