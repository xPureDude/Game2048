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
    template <typename T>
    static std::shared_ptr<T> CreateElement();

    std::shared_ptr<Element> CreateElement(ElementType type, const std::string& name);
};

template <typename T>
std::shared_ptr<T> ElementFactory::CreateElement()
{
    return std::make_shared<T>();
}

} // namespace gui

#endif // ELEMENTFACTORY_HPP