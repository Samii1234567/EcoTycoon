#ifndef GAMEBACKGROUND_H
#define GAMEBACKGROUND_H

#include <SFML/Graphics.hpp>
#include <string>

// ===================================================================================
//
//  PLIK NAGŁÓWKOWY DLA KLASY GAMEBACKGROUND
//
//  Prosta klasa, której jedynym zadaniem jest wczytanie i narysowanie
//  grafiki tła dla głównego ekranu rozgrywki.
//
// ===================================================================================

class GameBackground {
public:
    // --- Konstruktory ---
    GameBackground() = default;
    ~GameBackground() = default;

    // --- Metody publiczne ---

    // Wczytuje teksturę tła z podanej ścieżki pliku.
    // Zwraca `true`, jeśli wczytywanie się powiodło, w przeciwnym razie `false`.
    bool loadFromFile(const std::string& filename) {
        if (!m_texture.loadFromFile(filename))
            return false;

        m_sprite.setTexture(m_texture);

        // Skaluje wczytaną grafikę, aby dopasować ją do wymiarów okna (1200x800).
        // Dzięki temu obraz tła zawsze wypełni całe okno.
        auto [w, h] = m_texture.getSize();
        m_sprite.setScale(1200.f / float(w), 800.f / float(h));

        return true;
    }

    // Rysuje tło w podanym oknie.
    void draw(sf::RenderWindow& window) const {
        window.draw(m_sprite);
    }

private:
    // --- Prywatne pola klasy ---

    sf::Texture m_texture; // Tekstura przechowująca wczytaną grafikę tła.
    sf::Sprite  m_sprite;  // Obiekt Sprite, który jest rysowany na ekranie.
};

#endif // GAMEBACKGROUND_H
