#pragma once

#include "../pch.hpp" // IWYU pragma: keep
#include "Element.hpp"

namespace gui
{

class Button : public Element
{
public:
    Button();
    virtual ~Button();
};

} // namespace gui