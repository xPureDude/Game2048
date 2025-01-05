#ifndef LABEL_HPP
#define LABEL_HPP

#include "Element.hpp"

namespace gui
{

struct LabelInfo
{
    // base
    sf::Vector2f m_scale{1.f, 1.f};

    // shape
    float m_outlineSize{0.f};
    sf::Color m_color;
    sf::Color m_outlineColor;

    // texture
    std::shared_ptr<sf::Texture> m_texture{nullptr};
    sf::IntRect m_rect;
};

class Label : public Element
{
public:
    Label();
    virtual ~Label();

    void SetLabelInfo(const LabelInfo& info);

private:
    virtual void _RenderPrimitive(sf::RenderTarget* target) override;
    virtual void _UpdatePosition() override;
    virtual void _UpdateSize() override;

    LabelInfo m_info;
    sf::RectangleShape m_shape;
};

} // namespace gui

#endif // LABEL_HPP