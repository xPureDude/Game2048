#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "../pch.hpp" // IWYU pragma: keep

class Window;

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
/*
    void SubscribeObserver(BaseObserver* observer);
    void UnSubscribeObserver(BaseObserver* observer);

private:
    void _NotifyObserverDirect();
    void _NotifyObserverQueued();

protected:
    std::vector<BaseObserver*> m_observers;
*/

class Element
{
public:
    Element(Element* parent = nullptr);
    virtual ~Element();

    virtual void Update(const sf::Time& elapsed);
    virtual void Render(sf::RenderTarget* target);
    virtual void HandleInput(const sf::Event& event) {};

    std::string GetName() { return m_name; }

protected:
    Element* m_parent;
    bool m_activate;

    std::string m_name;
    sf::Vector2f m_position; // leftTop corner
    sf::Vector2f m_size;
};

using ElementPtr = std::shared_ptr<Element>;

} // namespace gui

#endif // ELEMENT_HPP