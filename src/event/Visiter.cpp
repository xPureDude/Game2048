#include "Visiter.hpp"

InputVisiter::InputVisiter()
{
}

void InputVisiter::operator()(const sf::Event::KeyPressed& event)
{
}

void InputVisiter::operator()(const sf::Event::KeyReleased& event)
{
}

void InputVisiter::operator()(const sf::Event::MouseMoved& event)
{
}

void InputVisiter::operator()(const sf::Event::MouseWheelScrolled& event)
{
}

void InputVisiter::operator()(const sf::Event::MouseButtonPressed& event)
{
}

void InputVisiter::operator()(const sf::Event::MouseButtonReleased& event)
{
}

void InputVisiter::operator()(const sf::Event::JoystickConnected& event)
{
}

void InputVisiter::operator()(const sf::Event::JoystickDisconnected& event)
{
}

void InputVisiter::operator()(const sf::Event::JoystickMoved& event)
{
}

void InputVisiter::operator()(const sf::Event::JoystickButtonPressed& event)
{
}

void InputVisiter::operator()(const sf::Event::JoystickButtonReleased& event)
{
}

void InputVisiter::operator()(const sf::Event::TextEntered& event)
{
}

void InputVisiter::operator()(const sf::Event::TouchBegan& event)
{
}

void InputVisiter::operator()(const sf::Event::TouchMoved& event)
{
}

void InputVisiter::operator()(const sf::Event::TouchEnded& event)
{
}