#pragma once

#include "AnimationSheet.hpp"

class SpriteSheet : public AnimationSheet
{
public:
    SpriteSheet(TextureManager* manager);

    bool LoadFromXmlElement(tinyxml2::XMLElement* element);
};
