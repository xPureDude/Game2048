#ifndef INPUTBINDING_HPP
#define INPUTBINDING_HPP

#include "../pch.hpp" // IWYU pragma: keep

namespace ib
{

enum class BindType
{
    WindowClose,
    FullscreenToggle,

    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
};

class Binding
{
    using CallbackType = std::function<void()>;

public:
    Binding(BindType bind);
    virtual ~Binding();

    template <typename T>
    void operator()(const T& t);
    virtual void operator()(const sf::Event::Closed& event) {}
    virtual void operator()(const sf::Event::KeyPressed& event) {}
    virtual void operator()(const sf::Event::KeyReleased& event) {}
    virtual void operator()(const sf::Event::MouseMoved& event) {}
    virtual void operator()(const sf::Event::MouseWheelScrolled& event) {}
    virtual void operator()(const sf::Event::MouseButtonPressed& event) {}
    virtual void operator()(const sf::Event::MouseButtonReleased& event) {}
    virtual void operator()(const sf::Event::JoystickConnected& event) {}
    virtual void operator()(const sf::Event::JoystickDisconnected& event) {}
    virtual void operator()(const sf::Event::JoystickMoved& event) {}
    virtual void operator()(const sf::Event::JoystickButtonPressed& event) {}
    virtual void operator()(const sf::Event::JoystickButtonReleased& event) {}
    virtual void operator()(const sf::Event::TextEntered& event) {}
    virtual void operator()(const sf::Event::TouchBegan& event) {}
    virtual void operator()(const sf::Event::TouchMoved& event) {}
    virtual void operator()(const sf::Event::TouchEnded& event) {}

    void AddBindingCallback(const std::string_view& name, CallbackType callback);
    void DelBindingCallback(const std::string_view& name);
    void TriggerCallbacks();

    bool IsTriggered() { return m_isTriggered; }

protected:
    BindType m_bind;
    bool m_isTriggered;
    std::map<std::string_view, CallbackType> m_callbacks;
};

using BindingPtr = std::shared_ptr<Binding>;

template <typename T>
void Binding::operator()(const T& t)
{
}

class WindowClose : public Binding
{
public:
    WindowClose();

    virtual void operator()(const sf::Event::Closed& event) override;
};

class FullscreenToggle : public Binding
{
public:
    FullscreenToggle();

    virtual void operator()(const sf::Event::KeyPressed& event) override;
};

class MoveLeft : public Binding
{
public:
    MoveLeft();

    virtual void operator()(const sf::Event::KeyPressed& event) override;
};

class MoveRight : public Binding
{
public:
    MoveRight();

    virtual void operator()(const sf::Event::KeyPressed& event) override;
};

class MoveUp : public Binding
{
public:
    MoveUp();

    virtual void operator()(const sf::Event::KeyPressed& event) override;
};

class MoveDown : public Binding
{
public:
    MoveDown();

    virtual void operator()(const sf::Event::KeyPressed& event) override;
};

} // namespace ib

class InputBindingFactory final
{
public:
    ib::BindingPtr CreateInputBinding(ib::BindType type);
};

#endif // EVENTBINDING_HPP