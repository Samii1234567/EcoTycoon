#include "Grid.h"
#include "Constants.h"

Grid::Grid() : m_cols(0), m_rows(0), m_cellSize(0) {}

void Grid::initialize(sf::Vector2f position, int cols, int rows, float cellSize) {
    m_position = position;
    m_cols = cols;
    m_rows = rows;
    m_cellSize = cellSize;
    m_occupiedCells.assign(rows, std::vector<bool>(cols, false));
    m_gridLine.setFillColor(sf::Color(0, 0, 0, 40));
    m_highlightCell.setFillColor(sf::Color::Green);
}

sf::Vector2i Grid::worldToGridCoords(sf::Vector2f worldPosition) const {
    int col = static_cast<int>((worldPosition.x - m_position.x) / m_cellSize);
    int row = static_cast<int>((worldPosition.y - m_position.y) / m_cellSize);
    return {col, row};
}

sf::Vector2f Grid::gridToWorldCoords(sf::Vector2i gridPosition) const {
    float x = m_position.x + gridPosition.x * m_cellSize;
    float y = m_position.y + gridPosition.y * m_cellSize;
    return {x, y};
}

bool Grid::isAreaFree(sf::Vector2i gridPosition, sf::Vector2i size) const {
    for (int r = 0; r < size.y; ++r) {
        for (int c = 0; c < size.x; ++c) {
            int checkRow = gridPosition.y + r;
            int checkCol = gridPosition.x + c;
            if (checkRow < 0 || checkRow >= m_rows || checkCol < 0 || checkCol >= m_cols) {
                return false;
            }
            if (m_occupiedCells[checkRow][checkCol]) {
                return false;
            }
        }
    }
    return true;
}

void Grid::occupyArea(sf::Vector2i gridPosition, sf::Vector2i size) {
    for (int r = 0; r < size.y; ++r) {
        for (int c = 0; c < size.x; ++c) {
            int setRow = gridPosition.y + r;
            int setCol = gridPosition.x + c;
            if (setRow >= 0 && setRow < m_rows && setCol >= 0 && setCol < m_cols) {
                m_occupiedCells[setRow][setCol] = true;
            }
        }
    }
}

void Grid::freeArea(sf::Vector2i gridPosition, sf::Vector2i size) {
    for (int r = 0; r < size.y; ++r) {
        for (int c = 0; c < size.x; ++c) {
            int setRow = gridPosition.y + r;
            int setCol = gridPosition.x + c;
            if (setRow >= 0 && setRow < m_rows && setCol >= 0 && setCol < m_cols) {
                m_occupiedCells[setRow][setCol] = false;
            }
        }
    }
}

void Grid::draw(sf::RenderWindow& window, sf::Vector2f mousePosition, sf::Vector2i buildingSize) {
    for (int i = 0; i <= m_cols; ++i) {
        m_gridLine.setSize({1.f, m_rows * m_cellSize});
        m_gridLine.setPosition(m_position.x + i * m_cellSize, m_position.y);
        window.draw(m_gridLine);
    }
    for (int i = 0; i <= m_rows; ++i) {
        m_gridLine.setSize({m_cols * m_cellSize, 1.f});
        m_gridLine.setPosition(m_position.x, m_position.y + i * m_cellSize);
        window.draw(m_gridLine);
    }
    sf::Vector2i gridPos = worldToGridCoords(mousePosition);
    m_highlightCell.setSize({m_cellSize * buildingSize.x, m_cellSize * buildingSize.y});
    m_highlightCell.setPosition(gridToWorldCoords(gridPos));
    if (isAreaFree(gridPos, buildingSize)) {
        m_highlightCell.setFillColor(sf::Color(0, 255, 0, 80));
    } else {
        m_highlightCell.setFillColor(sf::Color(255, 0, 0, 80));
    }
    window.draw(m_highlightCell);
}
