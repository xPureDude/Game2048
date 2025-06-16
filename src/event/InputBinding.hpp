#ifndef Binding_HPP
#define Binding_HPP

#include <any>

#include "InputVisitorDependent.hpp"

namespace input
{

class Binding : public InputVisitorDependent
{
public:
    Binding();
    virtual ~Binding();

    void TriggerEvent();

    bool IsTriggered() { return m_isTriggered; }
    void SetTriggered(bool flag) { m_isTriggered = flag; }

protected:
    virtual void _DoTriggerEvent() = 0;

protected:
    bool m_isTriggered;
    std::any m_param;
};

class WindowClose : public Binding
{
public:
    virtual bool operator()(const sf::Event::Closed& event) override;

protected:
    virtual void _DoTriggerEvent() override;
};

class FullscreenToggle : public Binding
{
public:
    virtual bool operator()(const sf::Event::KeyPressed& event) override;

protected:
    virtual void _DoTriggerEvent() override;
};

class MouseMove : public Binding
{
public:
    virtual bool operator()(const sf::Event::MouseMoved& event) override;

protected:
    virtual void _DoTriggerEvent() override;

private:
    sf::Vector2f m_curPos;
    sf::Vector2f m_prevPos;
};

class MouseButtonDown : public Binding
{
public:
    virtual bool operator()(const sf::Event::MouseButtonPressed& event) override;

protected:
    virtual void _DoTriggerEvent() override;

private:
    sf::Mouse::Button m_button;
};

class MouseButtonUp : public Binding
{
public:
    virtual bool operator()(const sf::Event::MouseButtonReleased& event) override;

protected:
    virtual void _DoTriggerEvent() override;

private:
    sf::Mouse::Button m_button;
};

class MoveLeft : public Binding
{
public:
    virtual bool operator()(const sf::Event::KeyPressed& event) override;

protected:
    virtual void _DoTriggerEvent() override;
};

class MoveRight : public Binding
{
public:
    virtual bool operator()(const sf::Event::KeyPressed& event) override;

protected:
    virtual void _DoTriggerEvent() override;
};

class MoveUp : public Binding
{
public:
    virtual bool operator()(const sf::Event::KeyPressed& event) override;

protected:
    virtual void _DoTriggerEvent() override;
};

class MoveDown : public Binding
{
public:
    virtual bool operator()(const sf::Event::KeyPressed& event) override;

protected:
    virtual void _DoTriggerEvent() override;
};

} // namespace input

#endif // EVENTBINDING_HPP