#ifndef SCENE_GAMEOVER_HPP
#define SCENE_GAMEOVER_HPP

#include "Scene.hpp"

class SceneGameOver : public Scene
{
public:
    SceneGameOver(SceneManager* manager);
    virtual ~SceneGameOver();

    virtual bool OnCreate() override;
    virtual void OnDestroy() override;

    virtual void Update(const sf::Time& elasped) override;
    virtual void Render(Window* window) override;

    virtual void OnEnter(const std::any& param) override;
    virtual void OnLeave() override;

private:
    bool _InitGui();
    void _OnNewGame(const std::any& param);
    void _OnContinuePlay(const std::any& param);
    void _OnQuitGame(const std::any& param);
};

#endif // SCENE_GAMEOVER_HPP