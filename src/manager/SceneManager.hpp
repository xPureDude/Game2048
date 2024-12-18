#pragma once

#include "../core/Scene.hpp"

#include <functional>
#include <set>
#include <unordered_map>
#include <vector>

enum class SceneType
{
    None = 0,
    Intro,
    MainMenu,
    Play,
    Paused
};

using SceneTypeContainer = std::set<SceneType>;
using SceneFactory = std::unordered_map<SceneType, std::function<Scene*(void)>>;

struct SceneInfo
{
    SceneType m_type{SceneType::None};
    Scene* m_scene{nullptr};
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

    template <typename T>
    void _RegisterScene(SceneType type);

private:
    SharedContext* m_ctx;
    std::vector<SceneInfo> m_scenes;
    SceneTypeContainer m_removeLater;
    SceneFactory m_factory;
};

template <typename T>
void SceneManager::_RegisterScene(SceneType type)
{
    m_factory[type] = []() -> Scene* {
        auto scene = new T;
        return scene;
    };
}
