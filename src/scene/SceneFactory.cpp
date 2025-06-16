#include "SceneFactory.hpp"

#include "SceneGameOver.hpp"
#include "SceneMainMenu.hpp"
#include "ScenePlay.hpp"
#include "SceneSetting.hpp"
#include "common/Log.hpp"

std::unordered_map<SceneType, std::function<std::shared_ptr<Scene>(void)>> SceneFactory::m_registry{
    {SceneType::MainMenu, []() { return std::make_shared<SceneMainMenu>(); }},
    {SceneType::Setting, []() { return std::make_shared<SceneSetting>(); }},
    {SceneType::Play, []() { return std::make_shared<ScenePlay>(); }},
    {SceneType::GameOver, []() { return std::make_shared<SceneGameOver>(); }}};

std::shared_ptr<Scene> SceneFactory::CreateScene(SceneType sceneType)
{
    if (m_registry.contains(sceneType))
    {
        return m_registry[sceneType]();
    }
    DBG("SceneType {} not registered!", SceneType2String(sceneType));
    return nullptr;
}

bool SceneFactory::RegisterScene(SceneType sceneType, std::function<std::shared_ptr<Scene>(void)> func)
{
    auto ret = m_registry.try_emplace(sceneType, func);
    return ret.second;
}