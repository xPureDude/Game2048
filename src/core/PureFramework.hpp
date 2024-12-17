#pragma once

#include "SharedContext.hpp"

#include <SFML/Graphics.hpp>


class PureFramework
{
public:
    PureFramework();
    ~PureFramework();

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
