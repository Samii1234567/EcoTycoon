#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <optional>

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY BUILDMENU
//
//  Definiuje klasę `BuildMenu`, która odpowiada za interfejs menu budowania.
//  Menu to wyświetla listę dostępnych budynków, ich ceny i pozwala graczowi
//  na rozpoczęcie procesu budowy poprzez przeciągnięcie ikony na mapę.
//
// ===================================================================================


// Struktura przechowująca wszystkie informacje o pojedynczej pozycji w menu budowania.
struct BuildItem {
    int                  id;            // ID budynku, zgodne z GameConstants
    std::string          name;          // Wyświetlana nazwa
    int                  basePrice;     // Bazowa cena (nieużywane, ale może być przydatne)
    int                  currentPrice;  // Aktualna cena budowy
    sf::RectangleShape   buttonShape;   // Kształt przycisku w menu
    sf::Text             nameText;      // Tekst z nazwą na przycisku
    sf::Text             priceText;     // Tekst z ceną na przycisku
};

class BuildMenu {
public:
    // --- Typy publiczne ---

    // Typ wyliczeniowy określający rezultat obsługi kliknięcia w menu.
    enum class ClickResult { None, DragStarted, NotEnoughMoney };

    // Struktura przechowująca stan przeciągania budynku.
    // Jest tworzona, gdy gracz zaczyna przeciągać budynek z menu.
    struct DragState {
        int itemId;     // ID przeciąganego budynku
        int price;      // Cena, która zostanie odjęta po postawieniu
        sf::Sprite sprite;  // Duch" budynku podążający za kursorem
    };

    // --- Metody publiczne ---

    // Konstruktor
    BuildMenu();
    ~BuildMenu() = default;

    // Inicjalizuje menu, wczytując czcionkę i tekstury.
    bool initialize(const sf::Font& font, const std::vector<sf::Texture>& textures);

    // Ustawia widoczność menu.
    void setVisible(bool visible);

    // Zwraca informację, czy menu jest aktualnie widoczne.
    bool isVisible() const;

    // Przetwarza zdarzenia (głównie kliknięcia myszą) i zwraca rezultat.
    ClickResult handleEvent(const sf::Event& ev, sf::RenderWindow& window, int& money);

    // Rysuje menu budowania w podanym oknie.
    void draw(sf::RenderWindow& window);

    // Zwraca referencję do opcjonalnego stanu przeciągania.
    // `std::optional` jest używany, ponieważ przeciąganie nie zawsze jest aktywne.
    std::optional<DragState>& getDragState();

    // Anuluje proces przeciągania budynku.
    void cancelDragging();

    // Zwraca granice tła menu (przydatne do sprawdzania, czy kursor jest nad menu).
    sf::FloatRect getBackgroundBounds() const;

private:
    // --- Metody i pola prywatne ---

    // Prywatna metoda do skonfigurowania wszystkich pozycji w menu.
    void setupItems(const sf::Font& font);

    std::vector<BuildItem> items; // Wektor przechowujący wszystkie pozycje menu
    sf::RectangleShape     background; // Tło całego menu
    bool                   visible;    // Flaga widoczności

    // Wskaźnik do wektora tekstur budynków (przechowywanego w klasie Game).
    // Użycie wskaźnika zapobiega kopiowaniu ciężkich zasobów.
    const std::vector<sf::Texture>* itemTexturesRef = nullptr;

    // Opcjonalny obiekt stanu przeciągania.
    std::optional<DragState> dragState;

    // Pola do obsługi migania ceny na czerwono, gdy brakuje pieniędzy.
    int m_flashItemId = -1;
    sf::Clock m_flashClock;
};
