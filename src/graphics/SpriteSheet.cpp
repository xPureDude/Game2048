#include "SpriteSheet.hpp"

#include "../common/Log.hpp"
#include "../resource/TextureManager.hpp"
#include "GridAnimation.hpp"

SpriteSheet::SpriteSheet(TextureManager* manager)
    : AnimationSheet(manager)
{
}

bool SpriteSheet::LoadFromXmlElement(tinyxml2::XMLElement* e)
{
    m_name = e->Attribute("name");
    m_textureName = e->Attribute("texture_name");

    m_texture = m_textureManager->RequestResource(m_textureName);

    for (auto eChild = e->FirstChildElement(); eChild != nullptr; eChild = eChild->NextSiblingElement())
    {
        if (strcmp(eChild->Value(), "Animation") == 0)
        {
            GridAnimation* animate = new GridAnimation(this);
            if (animate->LoadFromXmlElement(eChild) == false)
            {
                ERR("AnimationSheet::_LoadSpriteSheetFromXml, Load Animation failed, SheetName: {}, TextureName: {}", m_name, m_textureName);
                delete animate;
                return false;
            }
            m_animations.emplace(animate->GetName(), animate);
        }
    }
    return true;
}
