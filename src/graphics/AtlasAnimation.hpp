#pragma once

#include "Animation.hpp"

class AtlasAnimation : public Animation
{
    struct Frame
    {
        bool m_flipped{false};
        sf::IntRect m_origin;
        sf::IntRect m_hurtBox;
        sf::IntRect m_hitBox;
    };

public:
    AtlasAnimation(AnimationSheet* sheet);
    virtual ~AtlasAnimation();

    virtual void OnDirectionChange(bool left) override;

    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* e) override;

private:
    virtual void _CropSprite() override;

private:
    std::vector<Frame> m_frames;
};