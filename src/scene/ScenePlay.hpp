#pragma once

#include "Scene.hpp"

class Game2048;

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
    void _InitGui();
    void _OnMoveLeft(const std::any& param);
    void _OnMoveRight(const std::any& param);
    void _OnMoveUp(const std::any& param);
    void _OnMoveDown(const std::any& param);

    void _OnScoreChange(const std::any& param);

private:
    Game2048* m_game2048;

    sf::RenderTexture m_boardTexture;
    sf::Sprite m_boardSprite;
    sf::RectangleShape m_blockBack;
};
