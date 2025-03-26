#include "GridAnimation.hpp"

#include "AnimationSheet.hpp"
#include "common/Log.hpp"


GridAnimation::GridAnimation(AnimationSheet* sheet)
    : Animation(sheet),
      m_spacing(0)
{
}

GridAnimation::~GridAnimation() {}

void GridAnimation::_CropSprite()
{
    sf::IntRect rect({m_originPos.x + m_curFrame * (m_size.x + m_spacing), m_originPos.y}, {m_size.x, m_size.y});
    m_sprite.setTextureRect(rect);
}

bool GridAnimation::LoadFromXmlElement(tinyxml2::XMLElement* e)
{
    m_name = e->Attribute("name");
    m_originPos.x = atoi(e->Attribute("origin_x"));
    m_originPos.y = atoi(e->Attribute("origin_y"));
    m_spacing = atoi(e->Attribute("spacing"));
    m_size.x = atoi(e->Attribute("frame_w"));
    m_size.y = atoi(e->Attribute("frame_h"));
    m_maxFrame = atoi(e->Attribute("frame_count")) - 1;
    m_frameTime = sf::milliseconds(atoi(e->Attribute("duration")));

    DBG("Load Animation {} success", m_name);
    return true;
}
