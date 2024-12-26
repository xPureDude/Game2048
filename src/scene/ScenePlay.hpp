#pragma once

#include "../gui/Element.hpp"
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
    void _OnMoveLeft();
    void _OnMoveRight();
    void _OnMoveUp();
    void _OnMoveDown();

private:
    Game2048* m_game2048;

    // UI
    gui::ElementPtr m_guiPanel;

    sf::RenderTexture m_boardTexture;
    sf::Sprite m_boardSprite;
    sf::RectangleShape m_blockBack;
};
