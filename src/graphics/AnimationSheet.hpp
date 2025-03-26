#pragma once

#include "SFML/Graphics.hpp" // IWYU pragma: keep
#include "SFML/System.hpp"   // IWYU pragma: keep
#include "common/predeclare.hpp"
#include "tinyxml/tinyxml2.h"

#include <string>

class AnimationSheet
{
public:
    AnimationSheet(TextureManager* manager);
    virtual ~AnimationSheet();

    void Update(const sf::Time& elapsed);
    void Render(Window* window);

    void SetDirection(bool left);
    void SetPosition(const sf::Vector2f& pos);
    void SetTextureName(const std::string& name);
    void SetCurAnimation(const std::string& name, bool loop = false);

    sf::Vector2f GetPosition();

    bool GetDirection() { return m_left; }
    sf::FloatRect GetGlobalBounds();
    sf::Vector2u GetTextureSize() { return m_texture->getSize(); }
    TextureManager* GetTextureManager() { return m_textureManager; }
    std::shared_ptr<sf::Texture> GetTexture() { return m_texture; }
    Animation* GetCurAnimation() { return m_curAnimation; }
    Animation* GetAnimation(const std::string& name);
    std::string GetSheetName() { return m_name; }

    bool IsCurAnimation(const std::string& name);
    void ActivateCurAnimation();

    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* elem) = 0;

protected:
    std::string m_name;
    TextureManager* m_textureManager;
    std::string m_textureName;
    std::shared_ptr<sf::Texture> m_texture;

    bool m_left;
    Animation* m_curAnimation;

    std::unordered_map<std::string, Animation*> m_animations;
};