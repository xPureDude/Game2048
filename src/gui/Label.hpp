#ifndef LABEL_HPP
#define LABEL_HPP

#include "Element.hpp"

namespace gui
{

class Label : public Element
{
public:
    Label();
    virtual ~Label();

    void SetLabelStyle(LabelStyle* style);

    LabelStyle* GetLabelStyle() { return m_style; }

private:
    virtual void _RenderPrimitive(sf::RenderTarget* target) override;
    virtual void _UpdatePosition() override;
    virtual void _UpdateSize() override;

    LabelStyle* m_style;
    sf::RectangleShape m_shape;
};

} // namespace gui

#endif // LABEL_HPP