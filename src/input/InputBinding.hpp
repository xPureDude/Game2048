#ifndef INPUTBINDING_HPP
#define INPUTBINDING_HPP

#include "../pch.hpp" // IWYU pragma: keep
#include "InputVisitorDependent.hpp"

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

class Binding : public InputVisitorDependent
{
public:
    Binding(BindType bind);
    virtual ~Binding();

    void AddBindingCallback(const std::string_view& name, CallbackType callback);
    void DelBindingCallback(const std::string_view& name);
    void TriggerCallbacks();

    bool IsTriggered() { return m_isTriggered; }

protected:
    BindType m_bind;
    bool m_isTriggered;
    std::any m_param;
    std::map<std::string_view, CallbackType> m_callbacks;
};

using BindingPtr = std::shared_ptr<Binding>;

class WindowClose : public Binding
{
public:
    WindowClose();

    virtual bool operator()(const sf::Event::Closed& event) override;
};

class FullscreenToggle : public Binding
{
public:
    FullscreenToggle();

    virtual bool operator()(const sf::Event::KeyPressed& event) override;
};

class MoveLeft : public Binding
{
public:
    MoveLeft();

    virtual bool operator()(const sf::Event::KeyPressed& event) override;
};

class MoveRight : public Binding
{
public:
    MoveRight();

    virtual bool operator()(const sf::Event::KeyPressed& event) override;
};

class MoveUp : public Binding
{
public:
    MoveUp();

    virtual bool operator()(const sf::Event::KeyPressed& event) override;
};

class MoveDown : public Binding
{
public:
    MoveDown();

    virtual bool operator()(const sf::Event::KeyPressed& event) override;
};

} // namespace ib

class InputBindingFactory final
{
public:
    ib::BindingPtr CreateInputBinding(ib::BindType type);
};

#endif // EVENTBINDING_HPP