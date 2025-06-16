#include "SpriteSheet.hpp"

#include "GridAnimation.hpp"
#include "common/Log.hpp"
#include "core/SharedContext.hpp"
#include "resource/TextureManager.hpp"

bool SpriteSheet::LoadFromXmlElement(tinyxml2::XMLElement* e)
{
    m_name = e->Attribute("name");
    m_textureName = e->Attribute("texture_name");

    m_texture = SharedContext::Instance()->Get<TextureManager>()->RequestResource(m_textureName);

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
