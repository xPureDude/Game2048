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
    void _OnMoveLeft(EventDetail* detail);
    void _OnMoveRight(EventDetail* detail);
    void _OnMoveUp(EventDetail* detail);
    void _OnMoveDown(EventDetail* detail);

private:
    Game2048 m_game2048;
};
