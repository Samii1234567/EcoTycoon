#include "Slider.h"
#include <algorithm> // Potrzebne dla funkcji std::clamp

// ===================================================================================
//
//  PLIK IMPLEMENTACYJNY DLA KLASY SLIDER
//
//  Zawiera logikę działania suwaka zadeklarowanego w Slider.h.
//
// ===================================================================================

// Implementacja konstruktora.
Slider::Slider(float sliderX, float sliderY, float width, float height, float thumbRadius)
    : _x(sliderX), _y(sliderY), _w(width), _h(height), _drag(false)
{
    // Konfiguracja wyglądu ścieżki suwaka (prostokąt).
    _track.setSize({width, height});
    _track.setPosition(sliderX, sliderY);
    _track.setFillColor(sf::Color(155, 182, 109)); // Kolor tła ścieżki

    // Konfiguracja wyglądu uchwytu suwaka (kółko).
    _thumb.setRadius(thumbRadius);
    _thumb.setOrigin(thumbRadius, thumbRadius); // Ustawia środek kółka w jego centrum
    _thumb.setFillColor(sf::Color(0, 136, 78));  // Kolor uchwytu

    // Ustawienie domyślnej wartości na 50% (środek suwaka).
    setValue(0.5f);
}

// Implementacja obsługi zdarzeń.
void Slider::handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
    // Sprawdzenie, czy lewy przycisk myszy został wciśnięty.
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        // Pobranie pozycji kursora w świecie gry.
        auto p = window.mapPixelToCoords({e.mouseButton.x, e.mouseButton.y});
        // Jeśli kursor jest nad uchwytem, rozpoczynamy przeciąganie.
        if (_thumb.getGlobalBounds().contains(p)) {
            _drag = true;
        }
    }
    // Sprawdzenie, czy lewy przycisk myszy został puszczony.
    else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
        // Kończymy przeciąganie.
        _drag = false;
    }
    // Sprawdzenie, czy mysz się poruszyła, a my jesteśmy w trybie przeciągania.
    else if (_drag && e.type == sf::Event::MouseMoved) {
        // Pobieramy pozycję X kursora.
        float mx = window.mapPixelToCoords({e.mouseMove.x, e.mouseMove.y}).x;
        // Ograniczamy pozycję X do granic ścieżki suwaka, aby uchwyt nie "wyskoczył".
        mx = std::clamp(mx, _x, _x + _w);
        // Ustawiamy nową pozycję uchwytu (tylko w osi X, Y jest stałe).
        _thumb.setPosition(mx, _y + _h / 2.f);
    }
}

// Implementacja rysowania.
void Slider::draw(sf::RenderWindow& window) const {
    // Rysujemy najpierw ścieżkę, a potem na niej uchwyt.
    window.draw(_track);
    window.draw(_thumb);
}

// Implementacja pobierania wartości.
float Slider::getValue() const {
    // Obliczamy pozycję względną uchwytu (od 0.0 do 1.0).
    // (aktualna pozycja X - pozycja startowa X) / szerokość suwaka
    return (_thumb.getPosition().x - _x) / _w;
}

// Implementacja ustawiania wartości.
void Slider::setValue(float value) {
    // Ograniczamy wartość wejściową do przedziału [0.0, 1.0].
    float v = std::clamp(value, 0.f, 1.f);
    // Obliczamy pozycję w pikselach na podstawie znormalizowanej wartości.
    float mx = _x + v * _w;
    // Ustawiamy pozycję uchwytu.
    _thumb.setPosition(mx, _y + _h / 2.f);
}
