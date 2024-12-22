#pragma once

#include "Scene.hpp"

using SceneTypeContainer = std::set<SceneType>;

struct SceneInfo
{
    SceneType m_type{SceneType::None};
    std::shared_ptr<Scene> m_scene;
};

class SharedContext;

class SceneManager
{
public:
    SceneManager(SharedContext* ctx);
    ~SceneManager();

    bool Init(SharedContext* context);
    void UnInit();

    void Update(const sf::Time& elapsed);
    void Render();

    void ChangeSceneTo(SceneType type);
    void RemoveScene(SceneType type);

    void ProcessRemoves();

    SharedContext* GetSharedContext() { return m_ctx; }

private:
    bool _CreateScene(SceneType type);
    void _DestroyScene(SceneType type);
    bool _IsInRemoveLater(SceneType type);

private:
    SharedContext* m_ctx;
    std::vector<SceneInfo> m_scenes;
    SceneTypeContainer m_removeLater;
    SceneFactory m_factory;
};