#include "AtlasPackage.hpp"

#include "../resource/TextureManager.hpp"
#include "AtlasAnimation.hpp"

AtlasPackage::AtlasPackage(TextureManager* manager)
    : AnimationSheet(manager)
{
}

bool AtlasPackage::LoadFromXmlElement(tinyxml2::XMLElement* elem)
{
    if (elem == nullptr)
        return false;

    m_name = elem->Attribute("name");
    m_textureName = elem->Attribute("collectionName");
    m_texture = m_textureManager->GetResource(m_textureName);

    tinyxml2::XMLElement* animateElem = nullptr;
    for (auto e = elem->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
        if (std::strcmp(e->Value(), "Animation") == 0)
        {
            if (m_animations.contains(e->Attribute("name")))
            {
                DBG("AtlasPackage::LoadFromXml, duplicante Animation, name: {}", e->Attribute("name"));
                return false;
            }
            AtlasAnimation* animation = new AtlasAnimation(this);
            if (animation->LoadFromXmlElement(e) == false)
            {
                delete animation;
                return false;
            }
            m_animations.emplace(animation->GetName(), animation);
        }
    }

    return true;
}