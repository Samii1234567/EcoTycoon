#ifndef GAMEBACKGROUND_H
#define GAMEBACKGROUND_H

#include <SFML/Graphics.hpp>
#include <string>

// Klasa odpowiada za wczytanie i rysowanie tła gry.
// Plik PNG powinien mieć wymiary 1200×800 px.
class GameBackground {
public:
    GameBackground() = default;
    ~GameBackground() = default;

    // Wczytuje teksturę z podanej ścieżki. Zwraca false, jeśli nie udało się wczytać.
    bool loadFromFile(const std::string& filename) {
        if (!m_texture.loadFromFile(filename))
            return false;
        m_sprite.setTexture(m_texture);
        // Zakładamy, że obraz ma już wymiary 1200×800 – nie skalujemy.
        // Jeśli wymiary byłyby inne, można dopasować:
        auto [w, h] = m_texture.getSize();
        m_sprite.setScale(1200.f / float(w), 800.f / float(h));
        return true;
    }

    // Rysuje tło gry w oknie.
    void draw(sf::RenderWindow& window) const {
        window.draw(m_sprite);
    }

private:
    sf::Texture m_texture;
    sf::Sprite  m_sprite;
};

#endif // GAMEBACKGROUND_H
