#pragma once

#include "AnimationSheet.hpp"

class AtlasPackage : public AnimationSheet
{
public:
    AtlasPackage(TextureManager* manager);

    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* elem) override;
};