#include "Element.hpp"

namespace gui
{

Element::Element(Element* parent)
    : m_parent(parent)
{
}

Element::~Element()
{
}

void Element::Update(const sf::Time& elapsed)
{
}

void Element::Render(sf::RenderTarget* target)
{
}

} // namespace gui