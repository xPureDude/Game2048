#include "AtlasAnimation.hpp"

#include "AnimationSheet.hpp"

AtlasAnimation::AtlasAnimation(AnimationSheet* sheet)
    : Animation(sheet)
{
}

AtlasAnimation::~AtlasAnimation() {}

void AtlasAnimation::OnDirectionChange(bool left)
{
    m_sprite.setScale({1.0f, 1.0f});
    if (m_frames[m_curFrame].m_flipped == true)
    {
        if (left)
        {
            m_sprite.setScale({1.0f, -1.0f});
        }
    }
    else
    {
        if (!left)
        {
            m_sprite.setScale({-1.f, 1.f});
        }
    }
}

bool AtlasAnimation::LoadFromXmlElement(tinyxml2::XMLElement* elem)
{
    if (elem == nullptr)
        return false;

    m_name = elem->Attribute("name");
    m_frameTime = sf::milliseconds(atoi(elem->Attribute("frameTime")));
    tinyxml2::XMLElement* frameElem = nullptr;
    for (auto e = elem->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (std::strcmp(e->Value(), "Frame") == 0)
        {
            Frame frame;
            frame.m_origin.position.x = atoi(e->Attribute("x"));
            frame.m_origin.position.y = atoi(e->Attribute("y"));
            frame.m_origin.size.x = atoi(e->Attribute("width"));
            frame.m_origin.size.y = atoi(e->Attribute("height"));
            frame.m_flipped = atoi(e->Attribute("flipped")) == 1 ? true : false;
            m_frames.push_back(std::move(frame));
        }
    }
    m_maxFrame = m_frames.size();

    return true;
}

void AtlasAnimation::_CropSprite()
{
    m_sprite.setTextureRect(m_frames[m_curFrame].m_origin);
    auto spriteSize = m_sprite.getLocalBounds();
    m_sprite.setOrigin(spriteSize.getCenter());
    m_sprite.setScale({1.0f, 1.0f});
    m_sprite.setRotation(sf::degrees(0));

    if (m_frames[m_curFrame].m_flipped == true)
    {
        if (m_sheet->GetDirection())
        {
            m_sprite.setScale({1.0f, -1.0f});
        }
        m_sprite.setRotation(sf::degrees(-90));
    }
    else
    {
        if (!m_sheet->GetDirection())
        {
            m_sprite.setScale({-1.0f, 1.0f});
        }
    }
}