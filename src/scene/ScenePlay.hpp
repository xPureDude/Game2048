#pragma once

#include "../gameplay/Game2048.hpp"
#include "Scene.hpp"

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
    void _OnMoveLeft();
    void _OnMoveRight();
    void _OnMoveUp();
    void _OnMoveDown();

private:
    Game2048 m_game2048;
};
