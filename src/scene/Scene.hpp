#pragma once

#include <any>

#include "SFML/Graphics/View.hpp"
#include "SFML/System/Time.hpp"
#include "SceneManager.hpp"
#include "event/EventListenable.hpp"

class Window;

class Scene : public EventListenable
{
public:
    Scene();
    virtual ~Scene();

    virtual bool OnCreate() = 0;
    virtual void OnDestroy() = 0;

    virtual void Update(const sf::Time& elasped) = 0;
    virtual void Render(Window* window) = 0;

    virtual void OnEnter(const std::any& param);
    virtual void OnLeave();

    bool IsUpdateTransparent() { return m_updateTransparent; }
    bool IsRenderTransparent() { return m_renderTransparent; }

    sf::View& GetView() { return m_view; }

protected:
    bool m_updateTransparent;
    bool m_renderTransparent;
    sf::View m_view;
};