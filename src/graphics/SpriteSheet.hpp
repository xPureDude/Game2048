#pragma once

#include "AnimationSheet.hpp"

class SpriteSheet : public AnimationSheet
{
public:
    bool LoadFromXmlElement(tinyxml2::XMLElement* element);
};
