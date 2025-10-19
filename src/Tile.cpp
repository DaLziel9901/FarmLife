#include "Tile.h"

Tile::Tile() : m_id(-1), m_walkable(true), m_plantable(false) {}
Tile::Tile(int id, bool walkable, bool plantable)
    : m_id(id), m_walkable(walkable), m_plantable(plantable) {
}

int Tile::getID() const { return m_id; }
void Tile::setID(int id) { m_id = id; }

bool Tile::isWalkable() const { return m_walkable; }
void Tile::setWalkable(bool value) { m_walkable = value; }

bool Tile::isPlantable() const { return m_plantable; }
void Tile::setPlantable(bool value) { m_plantable = value; }

const sf::Sprite& Tile::getSprite() const { return m_sprite; }
sf::Sprite& Tile::getSprite() { return m_sprite; }

void Tile::setSprite(const sf::Texture& texture, sf::IntRect rect) {
    m_sprite.setTexture(texture);
    m_sprite.setTextureRect(rect);
}

void Tile::setWorldPosition(float x, float y) {
    m_sprite.setPosition(x, y);
}
