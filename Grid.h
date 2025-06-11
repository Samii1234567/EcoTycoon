#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>
#include <vector>

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY GRID
//
//  Definiuje klasę `Grid`, która zarządza siatką do budowania w grze.
//  Odpowiada za śledzenie, które pola są zajęte, konwersję współrzędnych
//  oraz wizualizację siatki i dostępności terenu w trybie budowy.
//
// ===================================================================================

class Grid {
public:
    // --- Konstruktor ---
    Grid();

    // --- Metody publiczne ---

    // Inicjalizuje siatkę, ustawiając jej pozycję, wymiary i rozmiar komórki.
    void initialize(sf::Vector2f position, int cols, int rows, float cellSize);

    // Konwertuje współrzędne ze świata gry (w pikselach) na współrzędne siatki (indeksy kolumny i wiersza).
    sf::Vector2i worldToGridCoords(sf::Vector2f worldPosition) const;

    // Konwertuje współrzędne siatki (indeksy) na współrzędne w świecie gry (w pikselach).
    sf::Vector2f gridToWorldCoords(sf::Vector2i gridPosition) const;

    // Sprawdza, czy dany obszar na siatce (o określonej pozycji i rozmiarze) jest wolny.
    bool isAreaFree(sf::Vector2i gridPosition, sf::Vector2i size) const;

    // Oznacza dany obszar na siatce jako zajęty.
    void occupyArea(sf::Vector2i gridPosition, sf::Vector2i size);

    // Zwalnia dany obszar na siatce, oznaczając go jako dostępny.
    void freeArea(sf::Vector2i gridPosition, sf::Vector2i size);

    // Rysuje siatkę oraz podświetlenie pod kursorem w trybie budowy.
    // Kolor podświetlenia zależy od tego, czy można postawić budynek.
    void draw(sf::RenderWindow& window, sf::Vector2f mousePosition, sf::Vector2i buildingSize);

private:
    // --- Prywatne pola klasy ---

    sf::Vector2f m_position;  // Pozycja lewego górnego rogu siatki w oknie.
    int m_cols, m_rows;       // Liczba kolumn i wierszy siatki.
    float m_cellSize;         // Rozmiar boku pojedynczej komórki siatki w pikselach.

    // Dwuwymiarowa tablica (wektor wektorów) przechowująca stan każdej komórki (true - zajęta, false - wolna).
    std::vector<std::vector<bool>> m_occupiedCells;

    // Kształty używane do rysowania siatki i podświetlenia.
    sf::RectangleShape m_gridLine;      // Cienka linia do rysowania siatki.
    sf::RectangleShape m_highlightCell; // Półprzezroczysty prostokąt do podświetlania obszaru.
};

#endif // GRID_H
