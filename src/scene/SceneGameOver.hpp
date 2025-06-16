#ifndef SCENE_GAMEOVER_HPP
#define SCENE_GAMEOVER_HPP

#include "SFML/Graphics/RectangleShape.hpp"
#include "Scene.hpp"

class SceneGameOver : public Scene
{
    enum class State
    {
        Entering,
        Normal,
    };

public:
    SceneGameOver();
    virtual ~SceneGameOver();

    virtual bool OnCreate() override;
    virtual void OnDestroy() override;

    virtual void Update(const sf::Time& elasped) override;
    virtual void Render(Window* window) override;

    virtual void OnEnter(const std::any& param) override;
    virtual void OnLeave() override;

private:
    void _OnContinueGameEvent(evt::Base* event);
    void _OnRetryGameEvent(evt::Base* event);
    void _OnBackToMenuEvent(evt::Base* event);

private:
    bool m_isWin;
    State m_state;
    sf::Time m_fadeInElapsed;
    sf::RectangleShape m_background;
};

#endif // SCENE_GAMEOVER_HPP