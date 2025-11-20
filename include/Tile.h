#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Tile {
public:
    Tile();
    Tile(int id, bool walkable = true, bool plantable = false);

    // --- Getter & Setter ---
    int getID() const;
    void setID(int id);

    bool isWalkable() const;
    void setWalkable(bool value);

    bool isPlantable() const;
    void setPlantable(bool value);

    const sf::Sprite& getSprite() const;    
    sf::Sprite& getSprite();                

    void setSprite(const sf::Texture& texture, sf::IntRect rect);
    void setWorldPosition(float x, float y); // đặt vị trí ô plot trong world


private:
    int m_id;              
    bool m_walkable;        
    bool m_plantable;        
    sf::Sprite m_sprite;     
};
