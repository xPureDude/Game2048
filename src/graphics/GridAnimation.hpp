#pragma once

#include "Animation.hpp"

class GridAnimation : public Animation
{
public:
    GridAnimation(AnimationSheet* sheet);
    virtual ~GridAnimation();

    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* e) override;

private:
    virtual void _CropSprite() override;

private:
    std::int32_t m_spacing;
    sf::Vector2i m_originPos;
    sf::Vector2i m_size;
};
