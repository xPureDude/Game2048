#pragma once

#include <memory>
#include <vector>

#include "SFML/Graphics.hpp" // IWYU pragma: keep
#include "SFML/System.hpp"   // IWYU pragma: keep
#include "event/EventListenable.hpp"

namespace evt
{
class Base;
}

namespace gui
{
class Element;

class Layer : public EventListenable
{
public:
    Layer();
    virtual ~Layer();

    virtual void UpdateFrame(const sf::Time& elapsed);
    virtual void Render(sf::RenderTarget* target);

    void SetActivate(bool flag);
    bool GetActivate() { return m_isActivate; }

    std::shared_ptr<gui::Element> AddElement(const std::string& name);
    void DelElement(const std::string& name);

protected:
    virtual void OnEventMouseMove(evt::Base* event);
    virtual void OnEventMouseDown(evt::Base* event);
    virtual void OnEventMouseUp(evt::Base* event);
    virtual void OnEventMouseScroll(evt::Base* event);

private:
    void _OnLanguageChanged(evt::Base* event);

protected:
    std::vector<std::shared_ptr<gui::Element>> m_elements;

private:
    bool m_isActivate;
};

/*
template <typename... ParamList>
std::shared_ptr<gui::Element> Layer::AddElement(const ParamList&&... params)
{
    auto elem = std::make_shared<gui::Element>();
    m_elements.push_back(elem);
    return elem;
}
*/

} // namespace gui