#include "Animation.hpp"

#include "AnimationSheet.hpp"

#include <cassert>

Animation::Animation(AnimationSheet* sheet)
    : m_isLoop(false),
      m_isPlay(false),
      m_isPause(false),
      m_curFrame(0),
      m_maxFrame(0),
      m_sheet(sheet)
{
    assert(sheet != nullptr);
    m_sprite.setTexture(sheet->GetTexture());
}

Animation::~Animation() {}

void Animation::Reset()
{
    m_curFrame = 0;
    m_elapsedTime = sf::Time();
    _CropSprite();
}

void Animation::Update(sf::Time elasped)
{
    if (m_isPlay == false || m_isPause == true)
        return;

    m_elapsedTime += elasped;
    if (m_elapsedTime > m_frameTime)
    {
        ++m_curFrame;
        m_elapsedTime -= m_frameTime;
        if (m_isLoop == false && m_curFrame == m_maxFrame)
        {
            --m_curFrame;
            m_isPlay = false;
            m_elapsedTime = sf::Time();
            return;
        }
        else
        {
            if (m_curFrame == m_maxFrame)
                m_curFrame = 0;
            _CropSprite();
        }
    }
}
