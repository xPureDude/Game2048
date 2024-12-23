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

} // namespace gui