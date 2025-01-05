#ifndef ELEMENTFACTORY_HPP
#define ELEMENTFACTORY_HPP

#include "Element.hpp"

namespace gui
{

enum class ElementType
{
    // Common Element,
    Button,
    Label,

    // Container
    Widget,
};

class ElementFactory
{
public:
    static std::shared_ptr<Element> CreateElement(ElementType type, GuiManager* manager = nullptr);
};

} // namespace gui

#endif // ELEMENTFACTORY_HPP