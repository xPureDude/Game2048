#pragma once

#include "AnimationSheet.hpp"

#include <SFML/Graphics.hpp>

class Window;
class TextureManager;

class SpriteSheet : public AnimationSheet
{
public:
    SpriteSheet(TextureManager* manager);

    bool LoadFromXmlElement(tinyxml2::XMLElement* element);
};
