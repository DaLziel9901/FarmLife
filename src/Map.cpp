#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>

Map::Map() : m_width(0), m_height(0) {}

bool Map::load(const std::string& tilesetPath, sf::Vector2u tileSize,
    const std::vector<int>& tiles, unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    m_tileSize = tileSize;

    // Load tileset
    if (!m_tileset.loadFromFile(tilesetPath)) {
        std::cerr << "Failed to load tileset: " << tilesetPath << std::endl;
        return false;
    }

    m_tiles.resize(height, std::vector<Tile>(width));

    unsigned int tilesPerRow = m_tileset.getSize().x / tileSize.x;

    // Gán dữ liệu cho từng tile
    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            int id = tiles[x + y * width];

            // Tạo tile
            Tile& tile = m_tiles[y][x];
            tile.setID(id);
            tile.setWalkable(true);
            tile.setPlantable(id == 2); 

            // Tính vị trí trong tileset
            int tu = id % tilesPerRow;
            int tv = id / tilesPerRow;

            // Gán sprite
            tile.setSprite(m_tileset, sf::IntRect(
                tu * tileSize.x, tv * tileSize.y, tileSize.x, tileSize.y));

            // Đặt vị trí tile trong world
            float worldX = static_cast<float>(x * tileSize.x);
            float worldY = static_cast<float>(y * tileSize.y);
            tile.setWorldPosition(worldX, worldY);
        }
    }

    return true;
}

bool Map::loadFromCSV(const std::string& csvFile,
    const std::string& tilesetPath,
    sf::Vector2u tileSize,
    const std::string& layerName)
{
    TileLayer layer;
    layer.name = layerName.empty() ? csvFile : layerName;
    layer.tileSize = tileSize;

    // Load texture
    if (!layer.texture.loadFromFile(tilesetPath)) {
        std::cerr << "Failed to load tileset: " << tilesetPath << std::endl;
        return false;
    }

    // Đọc CSV
    std::ifstream file(csvFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV: " << csvFile << std::endl;
        return false;
    }

    std::vector<int> ids;
    std::string line;
    unsigned int width = 0, height = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string val;
        unsigned int count = 0;
        while (std::getline(ss, val, ',')) {
            if (!val.empty()) {
                // Tiled export: ô trống thường là -1; có tile là >= 0
                ids.push_back(std::stoi(val));
                ++count;
            }
        }
        if (width == 0) width = count;
        ++height;
    }
    file.close();

    if (width == 0 || height == 0) {
        std::cerr << "Invalid CSV (empty or malformed): " << csvFile << std::endl;
        return false;
    }

    // Lần đầu load layer CSV → cập nhật m_width/m_height (kích thước map)
    if (m_width == 0 || m_height == 0) {
        m_width = width;
        m_height = height;
        m_tileSize = tileSize;
    }
    else {
        // Nếu đã có kích thước map từ layer trước, xác nhận khớp
        if (m_width != width || m_height != height) {
            std::cerr << "CSV size mismatch in " << csvFile
                << " (expected " << m_width << "x" << m_height
                << ", got " << width << "x" << height << ")\n";
            // vẫn có thể tiếp tục, nhưng tốt nhất nên khớp kích thước
        }
    }

    layer.width = width;
    layer.height = height;

    // Dựng VertexArray (quads)
    layer.vertices.setPrimitiveType(sf::Quads);
    layer.vertices.resize(width * height * 4);

    unsigned int tilesPerRow = layer.texture.getSize().x / tileSize.x;

    for (unsigned int y = 0; y < height; ++y) {
        for (unsigned int x = 0; x < width; ++x) {
            int id = ids[x + y * width];

            // ô trống trong CSV = -1 → bỏ qua (để quad ẩn)
            if (id < 0) {
                // đặt quad rỗng (0 area) để không vẽ
                sf::Vertex* quad = &layer.vertices[(x + y * width) * 4];
                quad[0].position = quad[1].position = quad[2].position = quad[3].position = sf::Vector2f(0.f, 0.f);
                quad[0].texCoords = quad[1].texCoords = quad[2].texCoords = quad[3].texCoords = sf::Vector2f(0.f, 0.f);
                continue;
            }

            int tu = id % tilesPerRow;
            int tv = id / tilesPerRow;

            sf::Vertex* quad = &layer.vertices[(x + y * width) * 4];

            // Vị trí trên màn hình
            quad[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
            quad[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
            quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
            quad[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);

            // Vị trí trong tileset
            quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
        }
    }

    m_layers.push_back(std::move(layer));
    std::cout << "Loaded CSV layer: " << csvFile << " ("
        << width << "x" << height << ")\n";
    return true;
}

void Map::update(float deltaTime)
{
    // Nếu có animation tile, update ở đây
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Vẽ theo thứ tự đã load (bạn gọi loadFromCSV theo thứ tự Water → Terrain → FarmPlot → Building → Decor)
    if (!m_layers.empty()) {
        for (const auto& layer : m_layers) {
            states.texture = &layer.texture;
            target.draw(layer.vertices, states);
        }
        return;
    }

    // fallback: vẽ theo hệ thống Tile cũ (nếu dùng)
    for (unsigned int y = 0; y < m_height; ++y)
        for (unsigned int x = 0; x < m_width; ++x)
            target.draw(m_tiles[y][x].getSprite(), states);
}

void Map::clear()
{
    m_tiles.clear();
    m_layers.clear();
	m_width = m_height = 0;
}

Tile& Map::getTile(unsigned int x, unsigned int y)
{
    return m_tiles[y][x];
}

const Tile& Map::getTile(unsigned int x, unsigned int y) const
{
    return m_tiles[y][x];
}
