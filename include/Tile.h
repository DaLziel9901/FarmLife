#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// ========================== CLASS TILE ==========================
// Mỗi Tile đại diện cho 1 ô trong map, chứa id sprite và thông tin logic.
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

    const sf::Sprite& getSprite() const;    // dùng để draw 
    sf::Sprite& getSprite();                // dùng khi cần chỉnh sprite (setPosition, setColor, ...)

    void setSprite(const sf::Texture& texture, sf::IntRect rect);
    void setWorldPosition(float x, float y); // đặt vị trí ô trong world


private:
    int m_id;                // ID tile trong tileset
    bool m_walkable;         // Có thể đi qua không
    bool m_plantable;        // Có thể trồng cây không
    sf::Sprite m_sprite;     // Sprite đại diện (texture + rect)
};
