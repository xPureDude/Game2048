#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

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

    bool IsTransparent() { return m_isTransparent; }
    bool IsTranscent() { return m_isTranscent; }

    sf::View& GetView() { return m_view; }

protected:
    bool m_isTransparent;
    bool m_isTranscent;
    sf::View m_view;
    SceneManager* m_sceneManager;
};
