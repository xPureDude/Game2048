#pragma once

#include "AnimationSheet.hpp"

class AtlasPackage : public AnimationSheet
{
public:
    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* elem) override;
};