#ifndef ELEMENTFACTORY_HPP
#define ELEMENTFACTORY_HPP

#include <memory>
#include <string>

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

class Element;

class ElementFactory
{
public:
    static std::shared_ptr<Element> CreateElement(ElementType type, const std::string& name);
};

} // namespace gui

#endif // ELEMENTFACTORY_HPP