#ifndef PUREFRAMEWORK_HPP
#define PUREFRAMEWORK_HPP

#include "../pch.hpp" // IWYU pragma: keep
#include "SharedContext.hpp"

struct EventBinding;

class PureFramework
{
public:
    PureFramework();
    ~PureFramework();

    bool Init();
    void UnInit();

    void Run();

    void Update();
    void Render();
    void HandleEvent();
    void LateUpdate();

protected:
    SharedContext m_ctx;
    sf::Time m_fpsTime;
    sf::Time m_elasped;
    sf::Clock m_clock;
};

#endif // PUREFRAMEWORK_HPP