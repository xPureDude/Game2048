#pragma once

#include "Scene.hpp"
#include "gameplay/Game2048.hpp"

class ScenePlay : public Scene
{
public:
    ScenePlay();
    virtual ~ScenePlay();

    virtual bool OnCreate() override;
    virtual void OnDestroy() override;

    virtual void Update(const sf::Time& elapsed) override;
    virtual void Render(Window* window) override;

    virtual void OnEnter(const std::any& param) override;
    virtual void OnLeave() override;

private:
    bool _InitGui();
    void _OnMoveLeft(const std::any& param);
    void _OnMoveRight(const std::any& param);
    void _OnMoveUp(const std::any& param);
    void _OnMoveDown(const std::any& param);

    void _OnNewGame(const std::any& param);
    void _OnScoreChange(const std::any& param);
    void _OnGameOver(const std::any& param);

private:
    NewGameInfo m_info;
    Game2048* m_game2048;

    std::shared_ptr<sf::Texture> m_backgroundTexture;
    sf::RectangleShape m_background;
};
