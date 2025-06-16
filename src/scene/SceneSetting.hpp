#pragma once

#include "SFML/Graphics.hpp" // IWYU pragma: keep
#include "Scene.hpp"

class SceneSetting : public Scene
{
public:
    SceneSetting();
    virtual ~SceneSetting();

    virtual bool OnCreate() override;
    virtual void OnDestroy() override;

    virtual void Update(const sf::Time& elasped) override;
    virtual void Render(Window* window) override;

    virtual void OnEnter(const std::any& param) override;
    virtual void OnLeave() override;

private:
    void _OnBackToMenuEvent(evt::Base* event);

private:
    sf::RectangleShape m_background;
    std::shared_ptr<sf::Texture> m_backgroundTexture;
};