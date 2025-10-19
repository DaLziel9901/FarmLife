#include "Animation.h"

Animation::Animation(sf::Sprite& sprite, int frameWidth, int frameHeight, float switchTime)
    : m_sprite(sprite), m_frameWidth(frameWidth), m_frameHeight(frameHeight),
    m_switchTime(switchTime), m_totalTime(0.f), m_currentFrame(0)
{
    m_rect = sf::IntRect(0, 0, frameWidth, frameHeight);
    m_sprite.setTextureRect(m_rect);
}

void Animation::update(float deltaTime, int directionIndex, bool isMoving)
{
    m_totalTime += deltaTime;

    if (isMoving) {
        if (m_totalTime >= m_switchTime) {
            m_totalTime = 0.f;
            m_currentFrame = (m_currentFrame + 1) % 3; // 3 frame mỗi hướng
        }
    }
    else {
        m_currentFrame = 1; // frame idle giữa
    }

    m_rect.top = directionIndex * m_frameHeight;
    m_rect.left = m_currentFrame * m_frameWidth;
    m_sprite.setTextureRect(m_rect);
}
