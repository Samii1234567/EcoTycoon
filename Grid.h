#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>
#include <vector>

class Grid {
public:
    Grid();
    void initialize(sf::Vector2f position, int cols, int rows, float cellSize);
    sf::Vector2i worldToGridCoords(sf::Vector2f worldPosition) const;
    sf::Vector2f gridToWorldCoords(sf::Vector2i gridPosition) const;
    bool isAreaFree(sf::Vector2i gridPosition, sf::Vector2i size) const;
    void occupyArea(sf::Vector2i gridPosition, sf::Vector2i size);
    void freeArea(sf::Vector2i gridPosition, sf::Vector2i size);
    void draw(sf::RenderWindow& window, sf::Vector2f mousePosition, sf::Vector2i buildingSize);

private:
    sf::Vector2f m_position;
    int m_cols, m_rows;
    float m_cellSize;
    std::vector<std::vector<bool>> m_occupiedCells;
    sf::RectangleShape m_gridLine;
    sf::RectangleShape m_highlightCell;
};

#endif // GRID_H
