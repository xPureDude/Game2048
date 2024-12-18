#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class SceneManager;
class Window;

struct EventDetail;

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
