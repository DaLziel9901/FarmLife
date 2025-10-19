#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

class Camera {
public:
    Camera(float width, float height);

    void follow(const sf::Vector2f& targetPos,
        float mapWidth, float mapHeight,
        bool smooth = false);

    void applyTo(sf::RenderWindow& window);
    void setZoom(float factor);
    sf::Vector2f getCenter() const; 
    sf::Vector2f getSize() const;  
    const sf::View& getView() const { return m_view; }

private:
    sf::View m_view;
};

