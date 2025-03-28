#pragma once

#include <any>
#include <memory>

#include "SFML/Graphics/View.hpp"
#include "SFML/System/Time.hpp"

class Window;

enum class SceneType
{
    None = 0,
    Intro,
    MainMenu,
    Play,
    Paused,
    GameOver,
};

std::string_view TranslateSceneTypeToStringView(SceneType type);

class Scene
{
    friend class SceneManager;

public:
    Scene();
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
};

using ScenePtr = std::shared_ptr<Scene>;