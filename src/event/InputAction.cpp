#include "InputAction.hpp"

namespace input
{
namespace action
{

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool Action::CheckTriggered()
{
    for (auto& code : m_keys)
    {
        if (sf::Keyboard::isKeyPressed(code))
            return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
MoveLeft::MoveLeft()
    : Action()
{
    m_keys.push_back(sf::Keyboard::Scancode::Left);
    m_keys.push_back(sf::Keyboard::Scancode::A);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
MoveRight::MoveRight()
    : Action()
{
    m_keys.push_back(sf::Keyboard::Scancode::Right);
    m_keys.push_back(sf::Keyboard::Scancode::D);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
MoveUp::MoveUp()
    : Action()
{
    m_keys.push_back(sf::Keyboard::Scancode::Up);
    m_keys.push_back(sf::Keyboard::Scancode::W);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
MoveDown::MoveDown()
    : Action()
{
    m_keys.push_back(sf::Keyboard::Scancode::Down);
    m_keys.push_back(sf::Keyboard::Scancode::S);
}

} // namespace action
} // namespace input