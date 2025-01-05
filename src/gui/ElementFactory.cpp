#include "ElementFactory.hpp"

#include "../pch.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "Widget.hpp"

namespace gui
{

std::shared_ptr<Element> ElementFactory::CreateElement(ElementType type, GuiManager* manager)
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