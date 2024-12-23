#pragma once

#include "../pch.hpp" // IWYU pragma: keep

struct EventDetail;

enum class SceneType
{
    None = 0,
    Intro,
    MainMenu,
    Play,
    Paused
};

class Scene
{
    friend class SceneManager;

public:
    Scene();
    virtual ~Scene();

    virtual bool OnCreate(SceneManager* manager);
    virtual void OnDestroy() = 0;

    virtual void Update(const sf::Time& elasped) = 0;
    virtual void Render(Window* window) = 0;

    virtual void OnEnter() = 0;
    virtual void OnLeave() = 0;

    bool IsUpdateTransparent() { return m_updateTransparent; }
    bool IsRenderTransparent() { return m_renderTransparent; }

    sf::View& GetView() { return m_view; }

protected:
    bool m_updateTransparent;
    bool m_renderTransparent;
    sf::View m_view;
    SceneManager* m_sceneManager;
};

using ScenePtr = std::shared_ptr<Scene>;

class SceneFactory final
{
public:
    SceneFactory() = default;
    ~SceneFactory();

    std::shared_ptr<Scene> CreateScene(SceneType sceneType);
    void ReleaseScene(SceneType sceneType);

private:
    std::map<SceneType, std::shared_ptr<Scene>> m_scenes;
};