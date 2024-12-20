#include "AnimationSheet.hpp"

#include "../common/Log.hpp"
#include "../core/Window.hpp"
#include "../resource/TextureManager.hpp"
#include "Animation.hpp"

AnimationSheet::AnimationSheet(TextureManager* manager)
    : m_textureManager(manager),
      m_texture(nullptr),
      m_left(true),
      m_curAnimation(nullptr)
{
}

AnimationSheet::~AnimationSheet()
{
    for (auto& it : m_animations)
    {
        if (it.second != nullptr)
        {
            delete it.second;
        }
    }
    m_textureManager->ReleaseResource(m_textureName);
}

void AnimationSheet::Update(const sf::Time& elapsed)
{
    if (m_curAnimation != nullptr)
        m_curAnimation->Update(elapsed);
}

void AnimationSheet::Render(Window* window)
{
    if (m_curAnimation != nullptr)
        window->Render(m_curAnimation->GetSprite());
}

void AnimationSheet::SetPosition(const sf::Vector2f& pos)
{
    if (m_curAnimation != nullptr)
        m_curAnimation->GetSprite().setPosition(pos);
}

void AnimationSheet::SetDirection(bool left)
{
    if (m_left == left)
        return;

    m_left = left;
    if (m_curAnimation != nullptr)
        m_curAnimation->OnDirectionChange(m_left);
}

void AnimationSheet::SetTextureName(const std::string& name)
{
    m_textureName = m_name = name;
    m_texture = m_textureManager->GetResource(name);
    assert(m_texture != nullptr);
}

void AnimationSheet::SetCurAnimation(const std::string& name, bool loop)
{
    if (m_curAnimation != nullptr && m_curAnimation->GetName() == name)
        return;

    if (m_animations.find(name) != m_animations.end())
    {
        m_curAnimation = m_animations[name];
        m_curAnimation->Reset();
        m_curAnimation->Play();
        if (loop)
            m_curAnimation->SetLoop(loop);
        DBG("AnimationSheet::SetCurAnimation, name: {}", name);
    }
}
const sf::Vector2f& AnimationSheet::GetPosition()
{
    return m_curAnimation->GetSprite().getPosition();
}

sf::FloatRect AnimationSheet::GetGlobalBounds()
{
    return m_curAnimation->GetSprite().getGlobalBounds();
}

Animation* AnimationSheet::GetAnimation(const std::string& name)
{
    if (m_animations.find(name) != m_animations.end())
    {
        return m_animations[name];
    }
    return nullptr;
}

bool AnimationSheet::IsCurAnimation(const std::string& name)
{
    if (m_curAnimation != nullptr && m_curAnimation->GetName() == name)
        return true;
    return false;
}

void AnimationSheet::ActivateCurAnimation()
{
    m_curAnimation->Reset();
    m_curAnimation->Play();
}
