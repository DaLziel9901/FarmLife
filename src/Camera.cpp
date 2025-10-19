#include "Camera.h"

Camera::Camera(float width, float height) {
    m_view.setSize(width, height);
    m_view.setCenter(width / 2.f, height / 2.f);
}

void Camera::follow(const sf::Vector2f& targetPos,
    float mapWidth, float mapHeight,
    bool smooth)
{
    sf::Vector2f newCenter = targetPos;

    sf::Vector2f half = m_view.getSize() / 2.f;
    float left = half.x;
    float top = half.y;
    float right = mapWidth - half.x;
    float bottom = mapHeight - half.y;

    newCenter.x = std::clamp(newCenter.x, left, right);
    newCenter.y = std::clamp(newCenter.y, top, bottom);

    if (smooth) {
        sf::Vector2f diff = newCenter - m_view.getCenter();
        m_view.move(diff * 0.1f); 
    }
    else {
        m_view.setCenter(newCenter);
    }
}

void Camera::applyTo(sf::RenderWindow& window) {
    window.setView(m_view);
}

void Camera::setZoom(float factor) {
    m_view.zoom(factor);
}
sf::Vector2f Camera::getCenter() const {
    return m_view.getCenter();
}
sf::Vector2f Camera::getSize() const {
    return m_view.getSize();
}