#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "Element.hpp"

namespace gui
{

struct WidgetInfo
{
};

class Widget : public Element
{
public:
    Widget();
    virtual ~Widget();

    virtual void Update(const sf::Time& elapsed);

    void SetRedraw(bool flag = true);

    std::shared_ptr<Element> FindChild(const std::string& name);
    bool AppendChild(std::shared_ptr<Element> child);
    void RemoveChild(const std::string& name);

private:
    void _RedrawPanel();
    virtual void _RenderPrimitive(sf::RenderTarget* target);
    virtual void _UpdatePosition();
    virtual void _UpdateSize();

protected:
    bool m_needRedraw;
    sf::RenderTexture m_panelTexture;
    sf::Sprite m_panelSprite;

    std::vector<std::shared_ptr<Element>> m_childs;
};

} // namespace gui

#endif // WIDGET_HPP