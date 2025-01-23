#pragma once

#include "../pch.hpp" // IWYU pragma: keep

struct EventDetail;

enum class SceneType
{
    None = 0,
    Intro,
    MainMenu,
    Play,
    Paused,
    GameOver,
};

class Scene
{
    friend class SceneManager;

public:
    Scene(SceneManager* manager);
    virtual ~Scene();

    virtual bool OnCreate() = 0;
    virtual void OnDestroy() = 0;

    virtual void Update(const sf::Time& elasped) = 0;
    virtual void Render(Window* window) = 0;

    virtual void OnEnter(const std::any& param) = 0;
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
    std::shared_ptr<Scene> CreateScene(SceneType sceneType, SceneManager* manager);
};