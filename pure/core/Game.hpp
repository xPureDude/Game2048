#pragma once

#include <SFML/Graphics.hpp>

#include "SharedContext.hpp"

class Game
{
public:
    Game();
    ~Game();

    bool Init();

    void Run();

    void Update();
    void Render();
    void HandleEvent();
    void LateUpdate();

private:
    SharedContext m_ctx;
    sf::Time m_elasped;
    sf::Clock m_clock;
};

