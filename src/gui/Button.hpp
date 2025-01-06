#pragma once

#include "Element.hpp"

namespace gui
{

struct ButtonInfo
{
    // base
    sf::Vector2f m_scale{1.f, 1.f};

    // shape
    float m_outlineSize{0.f};
    sf::Color m_color;
    sf::Color m_outlineColor;

    // texture
    std::shared_ptr<sf::Texture> m_texture;
    sf::IntRect m_rect;
};

class Button : public Element
{
public:
    Button();
    virtual ~Button();

    virtual bool operator()(const sf::Event::MouseMoved& event) override;
    virtual bool operator()(const sf::Event::MouseButtonPressed& event) override;
    virtual bool operator()(const sf::Event::MouseButtonReleased& event) override;

    void SetButtonInfo(ElementState state, const ButtonInfo& info);
    ButtonInfo* GetButtonInfo(ElementState state);

private:
    virtual void _RenderPrimitive(sf::RenderTarget* target) override;
    virtual void _UpdatePosition() override;
    virtual void _UpdateSize() override;
    void _UpdateCurrentState();

private:
    std::map<ElementState, ButtonInfo> m_infos;
    sf::RectangleShape m_shape;
};

} // namespace gui