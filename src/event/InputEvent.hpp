#pragma once

#include "BaseEvent.hpp"
#include "SFML/System.hpp" // IWYU pragma: keep
#include "SFML/Window.hpp" // IWYU pragma: keep

namespace evt
{

struct MouseMove : public InputBase
{
    EVENT_STATIC_NAME(MouseMove);

    sf::Vector2f m_prevPos;
    sf::Vector2f m_curPos;
    sf::Vector2f m_deltaPos;
};

struct MouseButtonDown : public InputBase
{
    EVENT_STATIC_NAME(MouseButtonDown);

    sf::Mouse::Button m_button;
};

struct MouseButtonUp : public InputBase
{
    EVENT_STATIC_NAME(MouseButtonDown);

    sf::Mouse::Button m_button;
};

struct MoveLeft : public InputBase
{
    EVENT_STATIC_NAME(MoveLeft);
};

struct MoveRight : public InputBase
{
    EVENT_STATIC_NAME(MoveRight);
};

struct MoveUp : public InputBase
{
    EVENT_STATIC_NAME(MoveUp);
};

struct MoveDown : public InputBase
{
    EVENT_STATIC_NAME(MoveDown);
};

} // namespace evt