#pragma once

#include "../pch.hpp" // IWYU pragma: keep

class AnimationSheet;

class Animation
{
public:
    Animation(AnimationSheet* sheet);
    virtual ~Animation();

    void Update(sf::Time elasped);
    void Reset();

    void Play() { m_isPlay = true; }
    void Stop() { m_isPause = false; }
    void Pause() { m_isPause = true; }
    void SetLoop(bool flag) { m_isLoop = flag; }
    void Continue() { m_isPause = false; }
    bool IsPlay() { return m_isPlay; }
    bool IsStop() { return m_isPlay == false; }
    bool IsLastFrame() { return m_curFrame == m_maxFrame - 1; }

    void SetAnimationName(const std::string& name) { m_name = name; }
    const std::string& GetName() { return m_name; }
    void SetFrameTime(const sf::Time& frameTime) { m_frameTime = frameTime; }

    sf::Sprite& GetSprite() { return m_sprite; }
    uint32_t GetCurFrame() { return m_curFrame; }
    bool IsCurFrame(uint32_t frame) { return m_curFrame == frame; }

    virtual void OnDirectionChange(bool left) {}

    virtual bool LoadFromXmlElement(tinyxml2::XMLElement* element) = 0;

private:
    virtual void _CropSprite() = 0;

protected:
    bool m_isLoop;
    bool m_isPlay;
    bool m_isPause;

    std::int32_t m_curFrame;
    std::int32_t m_maxFrame;
    sf::Time m_frameTime;
    sf::Time m_elapsedTime;

    std::string m_name;
    sf::Sprite m_sprite;
    AnimationSheet* m_sheet;
};