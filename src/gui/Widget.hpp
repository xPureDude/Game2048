#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "Element.hpp"

namespace gui
{

class Widget : public Element
{
public:
    Widget();
    virtual ~Widget();

    virtual void Update(const sf::Time& elapsed) override;
    virtual bool HandleInput(const sf::Event& event) override;

    void SetWidgetStyle(WidgetStyle* style);

    WidgetStyle* GetWidgetStyle() { return m_style; }

    void SetRedraw(bool flag);

    std::shared_ptr<Element> FindChild(const std::string& name);
    bool AppendChild(std::shared_ptr<Element> child);
    void RemoveChild(const std::string& name);

private:
    void _RedrawPanel();
    virtual void _RenderPrimitive(sf::RenderTarget* target) override;
    virtual void _UpdatePosition() override;
    virtual void _UpdateSize() override;

protected:
    bool m_needRedraw;
    WidgetStyle* m_style;
    sf::RenderTexture m_panelTexture;
    sf::Sprite m_panelSprite;

    std::vector<std::shared_ptr<Element>> m_childs;
};

} // namespace gui

#endif // WIDGET_HPP