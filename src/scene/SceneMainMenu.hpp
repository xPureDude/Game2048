#ifndef SCENEMENU_HPP
#define SCENEMENU_HPP

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Scene.hpp"

class SceneMainMenu : public Scene
{
public:
    SceneMainMenu();
    virtual ~SceneMainMenu();

    virtual bool OnCreate() override;
    virtual void OnDestroy() override;

    virtual void Update(const sf::Time& elasped) override;
    virtual void Render(Window* window) override;

    virtual void OnEnter(const std::any& param) override;
    virtual void OnLeave() override;

private:
    bool _InitGui();
    void _OnBeginGame(const std::any& param);
    void _OnOpenSetting(const std::any& param);
    void _OnQuitGame(const std::any& param);

private:
    std::shared_ptr<sf::Texture> m_backgroundTexture;
    sf::RectangleShape m_background;
};

#endif // SCENEMENU_HPP