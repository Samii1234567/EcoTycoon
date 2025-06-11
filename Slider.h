#ifndef SLIDER_H
#define SLIDER_H

#include <SFML/Graphics.hpp>

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY SLIDER
//
//  Definiuje klasę `Slider`, która implementuje graficzny suwak.
//  Jest to komponent UI wielokrotnego użytku, który pozwala użytkownikowi
//  wybrać wartość z określonego przedziału poprzez przeciąganie uchwytu.
//  Wartość zwracana jest znormalizowana do przedziału [0.0, 1.0].
//
// ===================================================================================

class Slider {
public:
    // --- Metody publiczne ---

    // Konstruktor tworzący suwak.
    // sliderX, sliderY – pozycja lewego górnego rogu ścieżki suwaka.
    // width, height     – wymiary ścieżki suwaka.
    // thumbRadius       – promień okrągłego uchwytu.
    Slider(float sliderX, float sliderY, float width, float height, float thumbRadius);

    // Przetwarza zdarzenia (eventy) z okna, np. kliknięcia i ruch myszą.
    // Metoda ta odpowiada za logikę przeciągania uchwytu suwaka.
    void handleEvent(const sf::Event& e, const sf::RenderWindow& window);

    // Rysuje suwak (jego ścieżkę i uchwyt) w podanym oknie.
    void draw(sf::RenderWindow& window) const;

    // Zwraca aktualną wartość suwaka jako liczbę z przedziału od 0.0 do 1.0.
    float getValue() const;

    // Ustawia pozycję uchwytu na podstawie znormalizowanej wartości (od 0.0 do 1.0).
    void setValue(float value);

private:
    // --- Prywatne pola klasy ---

    sf::RectangleShape _track; // Kształt reprezentujący ścieżkę suwaka.
    sf::CircleShape    _thumb; // Kształt reprezentujący uchwyt suwaka.

    float _x, _y, _w, _h;      // Przechowuje pozycję i wymiary dla łatwiejszych obliczeń.
    bool _drag = false;        // Flaga, która jest `true`, gdy użytkownik przeciąga uchwyt.
};

#endif // SLIDER_H
