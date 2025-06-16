#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "SceneDefine.hpp"

class Scene;
class SceneFactory
{
public:
    static std::shared_ptr<Scene> CreateScene(SceneType sceneType);

    static bool RegisterScene(SceneType sceneType, std::function<std::shared_ptr<Scene>(void)> func);

private:
    static std::unordered_map<SceneType, std::function<std::shared_ptr<Scene>(void)>> m_registry;
};