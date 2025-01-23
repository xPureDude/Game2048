#pragma once

#include "Element.hpp"

namespace gui
{

class Button : public Element
{
public:
    Button();
    virtual ~Button();

    virtual bool operator()(const sf::Event::MouseButtonPressed& event) override;
    virtual bool operator()(const sf::Event::MouseButtonReleased& event) override;

    void SetButtonStyle(ElementState state, ButtonStyle* style);
    ButtonStyle* GetButtonStyle(ElementState state);

private:
    virtual void _RenderPrimitive(sf::RenderTarget* target) override;
    virtual void _UpdateCurrentState() override;
    virtual void _UpdatePosition() override;
    virtual void _UpdateSize() override;

private:
    std::map<ElementState, ButtonStyle> m_styles;
    sf::RectangleShape m_shape;
};

} // namespace gui