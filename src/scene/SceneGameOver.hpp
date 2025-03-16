#ifndef SCENE_GAMEOVER_HPP
#define SCENE_GAMEOVER_HPP

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
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
    void _OnFirstButtonClicked(const std::any& param);
    void _OnBackButtonClicked(const std::any& param);

private:
    bool m_isWin;
    sf::RectangleShape m_background;
    std::shared_ptr<sf::Font> m_font;
    std::shared_ptr<sf::Text> m_text;
};

#endif // SCENE_GAMEOVER_HPP