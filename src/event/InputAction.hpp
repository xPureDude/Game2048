#pragma once
#include <vector>

#include "SFML/Window.hpp" // IWYU pragma: keep

namespace input
{
namespace action
{

class Action
{
public:
    Action() = default;

    bool CheckTriggered();

protected:
    std::vector<sf::Keyboard::Scancode> m_keys;
};

class MoveLeft : public Action
{
public:
    MoveLeft();
};

class MoveRight : public Action
{
public:
    MoveRight();
};

class MoveUp : public Action
{
public:
    MoveUp();
};

class MoveDown : public Action
{
public:
    MoveDown();
};

} // namespace action
} // namespace input