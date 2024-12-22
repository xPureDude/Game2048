#pragma once

#include "../pch.hpp" // IWYU pragma: keep

namespace gui
{
enum class Signal
{
    OnHover,
    OnPress,
    OnRelease,
    OnClicked,
};

class BaseObserver
{
public:
    BaseObserver();
    virtual ~BaseObserver();
};

class Element
{
public:
    Element();
    virtual ~Element();

    void SubscribeObserver(BaseObserver* observer);
    void UnSubscribeObserver(BaseObserver* observer);

private:
    void _NotifyObserverDirect();
    void _NotifyObserverQueued();

protected:
    std::vector<BaseObserver*> m_observers;
};

class Button : public Element
{
public:
    Button();
    virtual ~Button();
};

} // namespace gui