#pragma once

#include <any>
#include <set>
#include <unordered_map>

#include "SFML/System.hpp" // IWYU pragma: keep
#include "SceneDefine.hpp"

namespace event
{
class Base;
}

class Scene;

class SceneManager
{
public:
    SceneManager() = default;
    ~SceneManager();

    void UpdateFrame(const sf::Time& elapsed);
    void Render();

    // Pop Current Scene && Push New Scene "type"
    void ChangeScene(SceneType type, const std::any& param);

    // Push New Scene on top
    void PushScene(SceneType type, const std::any& param);
    void PopScene();

    void ProcessRemoves();

private:
    // Pop Current Scene
    bool _CreateScene(SceneType type);
    void _DestroyScene(SceneType type);
    bool _IsInRemoveLater(SceneType type);

private:
    std::vector<SceneType> m_sceneVec;
    std::unordered_map<SceneType, std::shared_ptr<Scene>> m_sceneMap;

    std::set<SceneType> m_removeLater;
};