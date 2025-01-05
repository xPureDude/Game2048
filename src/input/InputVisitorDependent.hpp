#ifndef VISITOR_HPP
#define VISITOR_HPP

#include "SFML/Window/Event.hpp"

// Pre implement
class InputVisitorDependent
{
public:
    template <typename T>
    bool operator()(const T& t);
    virtual bool operator()(const sf::Event::Closed& event) { return false; }
    virtual bool operator()(const sf::Event::KeyPressed& event) { return false; }
    virtual bool operator()(const sf::Event::KeyReleased& event) { return false; }
    virtual bool operator()(const sf::Event::MouseMoved& event) { return false; }
    virtual bool operator()(const sf::Event::MouseWheelScrolled& event) { return false; }
    virtual bool operator()(const sf::Event::MouseButtonPressed& event) { return false; }
    virtual bool operator()(const sf::Event::MouseButtonReleased& event) { return false; }
    virtual bool operator()(const sf::Event::JoystickConnected& event) { return false; }
    virtual bool operator()(const sf::Event::JoystickDisconnected& event) { return false; }
    virtual bool operator()(const sf::Event::JoystickMoved& event) { return false; }
    virtual bool operator()(const sf::Event::JoystickButtonPressed& event) { return false; }
    virtual bool operator()(const sf::Event::JoystickButtonReleased& event) { return false; }
    virtual bool operator()(const sf::Event::TextEntered& event) { return false; }
    virtual bool operator()(const sf::Event::TouchBegan& event) { return false; }
    virtual bool operator()(const sf::Event::TouchMoved& event) { return false; }
    virtual bool operator()(const sf::Event::TouchEnded& event) { return false; }
};

template <typename T>
bool InputVisitorDependent::operator()(const T& t)
{
    return false;
}

#endif // VISITOR_HPP