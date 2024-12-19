#pragma once

#include "../pch.pch" // IWYU pragma: keep
#include "SharedContext.hpp"


class PureFramework
{
public:
    PureFramework();
    ~PureFramework();

    void Init();
    void UnInit();

    void Run();

    void Update();
    void Render();
    void HandleEvent();
    void LateUpdate();

protected:
    SharedContext m_ctx;
    sf::Time m_elasped;
    sf::Clock m_clock;
};
