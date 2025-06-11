#include "Grid.h"
#include "Constants.h"

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY GRID
//
//  Zawiera logikę metod zadeklarowanych w Grid.h.
//
// ===================================================================================

// Domyślny konstruktor.
Grid::Grid() : m_cols(0), m_rows(0), m_cellSize(0) {}

// Implementacja inicjalizacji siatki.
void Grid::initialize(sf::Vector2f position, int cols, int rows, float cellSize) {
    // Przypisanie podstawowych parametrów siatki.
    m_position = position;
    m_cols = cols;
    m_rows = rows;
    m_cellSize = cellSize;

    // Inicjalizacja wektora `m_occupiedCells` - tworzymy tablicę o wymiarach `rows` x `cols`
    // i wypełniamy ją wartościami `false`, co oznacza, że na początku wszystkie pola są wolne.
    m_occupiedCells.assign(rows, std::vector<bool>(cols, false));

    // Ustawienie domyślnego koloru dla linii siatki i podświetlenia.
    m_gridLine.setFillColor(sf::Color(0, 0, 0, 40)); // Ciemna, półprzezroczysta linia
    m_highlightCell.setFillColor(sf::Color::Green);   // Domyślnie zielony (zmieniany w `draw`)
}

// Implementacja konwersji współrzędnych: świat -> siatka.
sf::Vector2i Grid::worldToGridCoords(sf::Vector2f worldPosition) const {
    // Obliczamy indeks kolumny i wiersza przez odjęcie pozycji siatki i podzielenie przez rozmiar komórki.
    int col = static_cast<int>((worldPosition.x - m_position.x) / m_cellSize);
    int row = static_cast<int>((worldPosition.y - m_position.y) / m_cellSize);
    return {col, row};
}

// Implementacja konwersji współrzędnych: siatka -> świat.
sf::Vector2f Grid::gridToWorldCoords(sf::Vector2i gridPosition) const {
    // Obliczamy pozycję w pikselach przez pomnożenie indeksów przez rozmiar komórki i dodanie pozycji siatki.
    float x = m_position.x + gridPosition.x * m_cellSize;
    float y = m_position.y + gridPosition.y * m_cellSize;
    return {x, y};
}

// Implementacja sprawdzania dostępności obszaru.
bool Grid::isAreaFree(sf::Vector2i gridPosition, sf::Vector2i size) const {
    // Iterujemy przez wszystkie komórki, które zająłby budynek.
    for (int r = 0; r < size.y; ++r) {
        for (int c = 0; c < size.x; ++c) {
            int checkRow = gridPosition.y + r;
            int checkCol = gridPosition.x + c;

            // Sprawdzamy, czy komórka nie wychodzi poza granice siatki.
            if (checkRow < 0 || checkRow >= m_rows || checkCol < 0 || checkCol >= m_cols) {
                return false; // Obszar wychodzi poza siatkę.
            }
            // Sprawdzamy, czy komórka jest już zajęta.
            if (m_occupiedCells[checkRow][checkCol]) {
                return false; // Znaleziono zajętą komórkę.
            }
        }
    }
    // Jeśli pętle się zakończą, oznacza to, że cały obszar jest wolny.
    return true;
}

// Implementacja oznaczania obszaru jako zajęty.
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

// Implementacja oznaczania obszaru jako wolny.
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

// Implementacja rysowania siatki.
void Grid::draw(sf::RenderWindow& window, sf::Vector2f mousePosition, sf::Vector2i buildingSize) {
    // Rysowanie pionowych linii siatki.
    for (int i = 0; i <= m_cols; ++i) {
        m_gridLine.setSize({1.f, m_rows * m_cellSize});
        m_gridLine.setPosition(m_position.x + i * m_cellSize, m_position.y);
        window.draw(m_gridLine);
    }
    // Rysowanie poziomych linii siatki.
    for (int i = 0; i <= m_rows; ++i) {
        m_gridLine.setSize({m_cols * m_cellSize, 1.f});
        m_gridLine.setPosition(m_position.x, m_position.y + i * m_cellSize);
        window.draw(m_gridLine);
    }

    // Rysowanie podświetlenia pod kursorem.
    sf::Vector2i gridPos = worldToGridCoords(mousePosition);
    m_highlightCell.setSize({m_cellSize * buildingSize.x, m_cellSize * buildingSize.y});
    m_highlightCell.setPosition(gridToWorldCoords(gridPos));

    // Zmiana koloru podświetlenia w zależności od dostępności miejsca.
    if (isAreaFree(gridPos, buildingSize)) {
        m_highlightCell.setFillColor(sf::Color(0, 255, 0, 80)); // Zielony, jeśli wolne.
    } else {
        m_highlightCell.setFillColor(sf::Color(255, 0, 0, 80)); // Czerwony, jeśli zajęte.
    }
    window.draw(m_highlightCell);
}
