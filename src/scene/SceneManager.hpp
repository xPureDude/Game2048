#pragma once

#include <set>

#include "Scene.hpp"
#include "SceneDependent.hpp"


struct SceneInfo
{
    SceneType m_type{SceneType::None};
    std::shared_ptr<Scene> m_scene;

    bool operator==(const SceneInfo& other)
    {
        return m_type == other.m_type;
    }
};

class SceneManager
{
public:
    SceneManager() = default;
    ~SceneManager();

    void Update(const sf::Time& elapsed);
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
    std::vector<SceneInfo> m_scenes;
    std::set<SceneType> m_removeLater;
    SceneFactory m_factory;
};