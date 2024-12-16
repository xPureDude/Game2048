#pragma once

#include "Scene.hpp"

#include <SFML/Graphics.hpp>

class DesktopPet;

class ScenePlay : public Scene
{
public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual bool OnCreate(SceneManager* manager) override;
    virtual void OnDestroy() override;

    virtual void Update(const sf::Time& elapsed) override;
    virtual void Render(Window* window) override;

    virtual void OnEnter() override;
    virtual void OnLeave() override;

private:
    void _Pause(EventDetail* detail);

private:
    sf::Vector2u m_windowSize;
    DesktopPet* m_pet;
    sf::Time m_lostElapsed;
};
