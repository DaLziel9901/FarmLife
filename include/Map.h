#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Tile.h"

class Map : public sf::Drawable {
public:
    Map();

    // Load tileset + tạo map từ mảng id
    bool load(const std::string& tilesetPath, sf::Vector2u tileSize,
        const std::vector<int>& tiles, unsigned int width, unsigned int height);

    //Load layer map từ file CSV
    bool loadFromCSV(const std::string& csvFile,
        const std::string& tilesetPath,
        sf::Vector2u tileSize,
        const std::string& layerName = "");

    void update(float deltaTime); // update animation tile
    void clear();                 // xóa map

    unsigned int getWidth() const { return m_width; }
    unsigned int getHeight() const { return m_height; }

    Tile& getTile(unsigned int x, unsigned int y);
    const Tile& getTile(unsigned int x, unsigned int y) const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    struct TileLayer
    {
        std::string name;
        sf::VertexArray vertices;
        sf::Texture texture;                  // texture Layer đó
        sf::Vector2u tileSize;                // Kích thước mỗi tile
        unsigned int width = 0, height = 0;   // Số lượng ô theo X, Y
    };

    std::vector<std::vector<Tile>> m_tiles;
    sf::Texture m_tileset;

	//mảng các layer
    std::vector<TileLayer> m_layers;

	sf::Vector2u m_tileSize;
	unsigned int m_width, m_height;
};
