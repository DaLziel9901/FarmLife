#pragma once
#include <SFML/Graphics.hpp>

class Animation {
public:
    Animation(sf::Sprite& sprite, int frameWidth, int frameHeight, float switchTime);

    void update(float deltaTime, int directionIndex, bool isMoving);

private:
    sf::Sprite& m_sprite;
    sf::IntRect m_rect;
    float m_totalTime;
    float m_switchTime;
    int m_frameWidth, m_frameHeight;
    int m_currentFrame;
};
#pragma once
