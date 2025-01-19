#include "ElementFactory.hpp"

#include "../pch.hpp" // IWYU pragma: keep
#include "Button.hpp"
#include "Label.hpp"
#include "Widget.hpp"

namespace gui
{

std::shared_ptr<Element> ElementFactory::CreateElement(ElementType type)
{
    switch (type)
    {
    case gui::ElementType::Button:
        return std::make_shared<Button>();
    case gui::ElementType::Label:
        return std::make_shared<Label>();
    case gui::ElementType::Widget:
        return std::make_shared<Widget>();
    default:
        return nullptr;
    }
}

} // namespace gui