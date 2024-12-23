#ifndef VISITER_HPP
#define VISITER_HPP

#include "../pch.hpp" // IWYU pragma: keep

class InputVisiter
{
public:
    InputVisiter();

    void operator()(const sf::Event::KeyPressed& event);
    void operator()(const sf::Event::KeyReleased& event);
    void operator()(const sf::Event::MouseMoved& event);
    void operator()(const sf::Event::MouseWheelScrolled& event);
    void operator()(const sf::Event::MouseButtonPressed& event);
    void operator()(const sf::Event::MouseButtonReleased& event);
    void operator()(const sf::Event::JoystickConnected& event);
    void operator()(const sf::Event::JoystickDisconnected& event);
    void operator()(const sf::Event::JoystickMoved& event);
    void operator()(const sf::Event::JoystickButtonPressed& event);
    void operator()(const sf::Event::JoystickButtonReleased& event);
    void operator()(const sf::Event::TextEntered& event);
    void operator()(const sf::Event::TouchBegan& event);
    void operator()(const sf::Event::TouchMoved& event);
    void operator()(const sf::Event::TouchEnded& event);
};

#endif // VISITER_HPP