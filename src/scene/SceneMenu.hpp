#ifndef SCENEMENU_HPP
#define SCENEMENU_HPP

#include "../gui/Button.hpp"
#include "Scene.hpp"

class SceneMenu : public Scene
{
public:
    SceneMenu();
    virtual ~SceneMenu();

    virtual bool OnCreate(SceneManager* manager) override;
    virtual void OnDestroy() override;

    virtual void Update(const sf::Time& elasped) override;
    virtual void Render(Window* window) override;

    virtual void OnEnter() override;
    virtual void OnLeave() override;

private:
    gui::Button m_beginButton;
    gui::Button m_quitButton;
};

#endif // SCENEMENU_HPP