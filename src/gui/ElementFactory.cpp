#include "ElementFactory.hpp"

#include "Button.hpp"
#include "Label.hpp"
#include "Widget.hpp"

namespace gui
{

std::shared_ptr<Element> ElementFactory::CreateElement(ElementType type, const std::string& name)
{
    std::shared_ptr<Element> elem;
    switch (type)
    {
    case gui::ElementType::Button:
        elem = std::make_shared<Button>();
        break;
    case gui::ElementType::Label:
        elem = std::make_shared<Label>();
        break;
    case gui::ElementType::Widget:
        elem = std::make_shared<Widget>();
        break;
    default:
        elem = nullptr;
        break;
    }
    if (elem)
    {
        elem->SetName(name);
    }

    return elem;
}

} // namespace gui